/*
 * Copyright (c) 2022 Particle Industries, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Geofence.h"
#include <math.h>


constexpr double EARTH_RADIUS = 6371.0; /*!< Earth radius in units of kilometers */

void Geofence::init() {
    //clear out the previous geofence zone boundary states
    for(auto&& iter : GeofenceZoneStates) {
        iter.prev_event = GeofenceEventType::UNKNOWN;
    }
}

void Geofence::loop() {
    auto zone_index = 0;
    for(auto zone : GeofenceZones) {
        if(zone.enable) {
            // If the current geocoordinate doesn't meet the DOP requirement then there
            // is nothing to do
            if (_geofence_point.hdop > _maximumDop) {
                CallbackContext context;
                context.event_type = GeofenceEventType::POOR_LOCATION;
                context.index = zone_index;
                for(auto callback : EventCallback) {
                    callback(context);
                }
                continue; // Go to next zone
            }

            bool outside_geofence =
                (zone.shape_type == GeofenceShapeType::CIRCULAR) ?
                    IsCircularGeofenceOutside(zone) :
                        IsPolygonalGeofenceOutside(zone);
            auto zone_state = GeofenceZoneStates.at(zone_index);
            CallbackContext context;
            if(IsEventTriggered(outside_geofence, zone, zone_index)) {
                //distance is outside geofence
                if(outside_geofence) {
                    if(zone.outside_event) {
                        context.event_type = GeofenceEventType::OUTSIDE;
                        context.index = zone_index;
                        for(auto callback : EventCallback) {
                            callback(context);
                        }
                    }
                    if(zone.exit_event) {
                        if(zone_state.prev_event == GeofenceEventType::INSIDE) {
                            context.event_type = GeofenceEventType::EXIT;
                            context.index = zone_index;
                            for(auto callback : EventCallback) {
                                callback(context);
                            }
                        }
                    }
                    //Store the most recent event type for that zone
                    GeofenceZoneStates.at(zone_index).prev_event =
                                                GeofenceEventType::OUTSIDE;
                }
                //distance is inside geofence
                else {
                    if(zone.inside_event) {
                        context.event_type = GeofenceEventType::INSIDE;
                        context.index = zone_index;
                        for(auto callback : EventCallback) {
                            callback(context);
                        }
                    }
                    if(zone.enter_event) {
                        if(zone_state.prev_event ==
                            GeofenceEventType::OUTSIDE) {
                            context.index = zone_index;
                            context.event_type = GeofenceEventType::ENTER;
                            for(auto callback : EventCallback) {
                                callback(context);
                            }
                        }
                    }
                    //Store the most recent event type for that zone
                    GeofenceZoneStates.at(zone_index).prev_event =
                                                GeofenceEventType::INSIDE;
                }
            }
        }
        zone_index++;
    }
}

bool Geofence::AnyGeofenceEnabled() {
    for(auto iter : GeofenceZones) {
        if(iter.enable) {
            return true;
        }
    }
    return false;
}

int Geofence::RegisterGeofenceCallback(GeofenceEventCallback callback) {
    EventCallback.append(callback);
    return SYSTEM_ERROR_NONE;
}

bool Geofence::IsCircularGeofenceOutside(ZoneInfo& zone) {
    double distance;
    GpsDistance(zone.center_lat, zone.center_lon, _geofence_point.lat,
                _geofence_point.lon, distance);
    //outside geofence
    if(distance > zone.radius) {
        return true;
    }
    else {
        return false;
    }
}

bool Geofence::IsPolygonalGeofenceOutside(ZoneInfo& zone) {
    if(IsPointInPolygon(zone.polygon_points,
                    _geofence_point.lat,
                    _geofence_point.lon)) {
        return false;
    }
    else {
        return true;
    }
}

bool Geofence::IsEventTriggered(bool outside_geofence,
                        ZoneInfo& zone,
                        int zone_index) {
    bool returnval = false;
    bool stable =
        ((outside_geofence && GeofenceZoneStates.at(zone_index).pending_event == GeofenceEventType::OUTSIDE) ||
            (!outside_geofence && GeofenceZoneStates.at(zone_index).pending_event == GeofenceEventType::INSIDE)) ||
                (!zone.verification_time_sec);
    if(!GeofenceZoneStates.at(zone_index).pending_time_ms || !stable) {
        GeofenceZoneStates.at(zone_index).pending_event = (outside_geofence)?
                    GeofenceEventType::OUTSIDE : GeofenceEventType::INSIDE;
        GeofenceZoneStates.at(zone_index).pending_time_ms = System.millis();
    }
    if(System.millis() - GeofenceZoneStates.at(zone_index).pending_time_ms >=
            zone.verification_time_sec*1000 && stable) {
        returnval = true;
    }
    return returnval;
}

int Geofence::HowManyPolygonPointsEnabled(Vector<PolygonPoint>& poly_points) {
    int count = 0;

    for(auto point : poly_points) {
        if(point.enable) {count++;}
    }

    return count;
}

double Geofence::CalculateLonDatelineOffset(Vector<PolygonPoint>& poly_points) {
    double offset = 0.0, min = poly_points.at(0).lon,
        max = poly_points.at(0).lon;

    //find the min and max longitude
    for(auto point : poly_points) {
        if(point.enable) {
            if(point.lon < min) {min = point.lon;}
            if(point.lon > max) {max = point.lon;}
        }
    }

    //is the longitude distance covered greater than 180 degrees
    if(fabs((min-max)) > 180.0) {
        offset = 360.0;
    }

    return offset;
}

void Geofence::GpsDistance(double las,
                            double los,
                            double lae,
                            double loe,
                            double& d) {
    double df, dfi, a;

    /* Convert degrees to radians */
    df = D2R(lae - las);
    dfi = D2R(loe - los);
    las = D2R(las);
    los = D2R(los);
    lae = D2R(lae);
    loe = D2R(loe);

    /*
    * Calculate distance
    *
    * Calculated distance is absolute value in meters between 2 points on earth.
    *
    *
    * a = sin(df / 2)^2 + cos(las) * cos(lae) * sin(dfi / 2)^2
    * d = RADIUS * 2 * atan(a / (1 - a)) * 1000 (for meters)
    */
    a = (double)(sin(df * 0.5) * sin(df * 0.5) + sin(dfi * 0.5) * sin(dfi * 0.5) * cos(las) * cos(lae));
    d = (double)(EARTH_RADIUS * 2.0 * atan2(sqrt(a), sqrt(1.0 - a)) * 1000.0);
}

bool Geofence::IsPointInPolygon(Vector<PolygonPoint>& poly_points,
                    double point_lat,
                    double point_lon) {
    double offset;
    int num_points_enabled = HowManyPolygonPointsEnabled(poly_points);
    int i, j= num_points_enabled-1;
    bool  odd_nodes = false;

    offset = CalculateLonDatelineOffset(poly_points);
    if(point_lon < 0.0) {point_lon += offset;}

    if(num_points_enabled) {
        //vector of only enabled polygon points
        Vector<PolygonPoint> enabled_poly_points;
        for(auto iter : poly_points) {
            if(iter.enable) {enabled_poly_points.append(iter);}
        }

        for(i=0; i < num_points_enabled; i++) {
            double point_i_lon = enabled_poly_points.at(i).lon;
            double point_j_lon = enabled_poly_points.at(j).lon;

            if(point_i_lon < 0.0) {point_i_lon += offset;}
            if(point_j_lon < 0.0) {point_j_lon += offset;}

            //is point latitude between polygon line segment
            if((enabled_poly_points.at(i).lat < point_lat &&
                enabled_poly_points.at(j).lat >= point_lat) ||
                    (enabled_poly_points.at(j).lat < point_lat &&
                        enabled_poly_points.at(i).lat >= point_lat)) {
                //is point to the right of polygon line segment
                //lonp > (lon1-lon2)*(latp-lat2)/(lat1-lat2)+lon2
                if(point_lon > (point_j_lon+(point_i_lon-point_j_lon)*
                        (point_lat-enabled_poly_points.at(j).lat)/
                            (enabled_poly_points.at(i).lat-enabled_poly_points.at(j).lat))) {
                    odd_nodes=!odd_nodes;
                }
            }
            j=i;
        }
    }

    return odd_nodes;
}






