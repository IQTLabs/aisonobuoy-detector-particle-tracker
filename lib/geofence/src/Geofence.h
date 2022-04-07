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
#pragma once

#include "Particle.h"
#include <atomic>

//forward declaration of struct and enum class
struct CallbackContext;
enum class GeofenceEventType;

/**
 * @brief Default maximum dilution of precison that can be used in
 * geolocation points
 *
 */
constexpr double GEOFENCE_MAXIMUM_DOP = 7.5; // Middle of moderate DOP values

/**
 * @brief Type definition of geofence event callback signature.
 *
 */
using GeofenceEventCallback =
        std::function<void(CallbackContext& context)>;

/**
 * @brief Max number of polygon points that can be used
 *
 */
constexpr int NUM_OF_POLYGON_POINTS = 10;

enum class GeofenceEventType {
    UNKNOWN,                ///< Unknown event type
    POOR_LOCATION,          ///< The current location doesn't pass evaluation quality
    INSIDE,                 ///< The current location is inside of the zone
    OUTSIDE,                ///< The current location is outside of the zone
    ENTER,                  ///< The current location has entered the zone
    EXIT,                   ///< The current location has exited the zone
};

struct PointData {
    double lat; /**< Point latitude in degrees */
    double lon; /**< Point longitude in degrees */
    double horizontal_accuracy; /**< error value */
    double hdop; /**<horizontal dilution of prescion */
    time_t gps_time;
    bool operator!=(const PointData& other) const {
        if((lat != other.lat) || (lon != other.lon)) {return true;}
        else {return false;}
    }
};

struct PolygonPoint {
    double lat{0.0};
    double lon{0.0};
    bool enable{false};
};

enum class GeofenceShapeType {
    CIRCULAR,
    POLYGONAL,
};

struct ZoneInfo {
    double radius{0.0}; //radius in meters that define the geofence zone boundary
    double center_lat{0.0};                 /**< Center point latitude in degrees */
    double center_lon{0.0};                /**< Center point longitude in degrees */
    Vector<PolygonPoint> polygon_points;
    bool enable{false}; //enable or disable the geofence zone
    bool inside_event{false};
    bool outside_event{false};
    bool enter_event{false};
    bool exit_event{false};
    uint32_t verification_time_sec{0};
    GeofenceShapeType shape_type{GeofenceShapeType::CIRCULAR};
};

struct CallbackContext {
    int index; //index of zone (+1 to get the actual zone number)
    GeofenceEventType event_type; //type of event that caused callback
};

struct GeofenceZoneState {
    GeofenceEventType prev_event{GeofenceEventType::UNKNOWN};
    GeofenceEventType pending_event{GeofenceEventType::UNKNOWN};
    uint64_t pending_time_ms{0};
};

class Geofence {
public:

    Geofence(int num_of_zones) : GeofenceZones(num_of_zones),
        GeofenceZoneStates(num_of_zones), _maximumDop(GEOFENCE_MAXIMUM_DOP) {
    }

    /**
     * @brief Initilize the geofence interface
     *
     * @details Sets the GeofenceZoneStates to GeofenceEventType::UNKNOWN in
     * order to be ready to capture ENTER and EXIT events
     */
    void init();

    /**
     * @brief Called periodically to check for geofence boundary conditions
     *
     * @details This is periodically called to calculate geofence points
     * and their relation to the boundary. Callbacks will be triggered here if
     * the event type conditions are met (outside, inside, enter, exit)
     */
    void loop();

    /**
     * @brief Sets the zone info for configuration of a zone
     *
     * @details Number of zones are created by the Geofence ctor, then they have
     * to be configured through this function, and placed in the vector using
     * the index passed to it
     *
     * @param[in] index index of vector to store the zone info
     * @param[in] zone_config reference to the zone info you want to set to
     */
    void SetZoneInfo(int index, const ZoneInfo& zone_config) {
        auto zone = GeofenceZones.at(index);
        zone = zone_config;
    }

    /**
     * @brief Gets the zone info for a given index
     *
     * @details Number of zones are created by the Geofence ctor, and this
     * function returns references to the zone info
     *
     * @param[in] index index of vector to get the zone info
     *
     * @return reference to requested zone info
     */
    ZoneInfo& GetZoneInfo(int index) {
        return GeofenceZones.at(index);
    }

    /**
     * @brief Is any geofence zone enabled
     *
     * @details Check all the geofence zones, and see if any are enabled
     *
     * @return true if any enabled, false if none enabled
     */
    bool AnyGeofenceEnabled();

    /**
     * @brief Pass the point data to be used to calculate boundary. Only writes
     * if there is a change to the stored _geofence_point
     *
     * @details This function is called to pass point information that is
     * used in the tick() function to calculate geofence bounds
     *
     * @param[in] PointData point to be passed for calculation
     */
    void UpdateGeofencePoint(const PointData& point) {
        _geofence_point = point;
    }

    /**
     * @brief Register a callback to occur for any geofence event
     *
     * @details Registers a callback to be triggered when any geofence event.
     * occurs. The callback will contain a context with the specific index and
     * event type that triggered the callback
     *
     * @param[in] callback function signature contain the context with the index
     * and event type that triggered the callback
     *
     * @return SYSTEM_ERROR_NONE
     */
    int RegisterGeofenceCallback(GeofenceEventCallback callback);

    /**
     * @brief Set the maximum HDOP figure any given location must have before a
     * geofence can be evaluated
     *
     * @param[in] dop maximum dilution of precision
     */
    void SetMaximumHdopLevel(double dop) {
        // Technically ranges from 0+ to 100 but allow all positive values
        _maximumDop = abs(dop);
    }

private:

    /**
     * @brief Checks if the circular geofence is outside the circle boundary
     *
     * @details Calculates the distance from the center of the boundary to the
     * given point and compares it to the radius of the circle. If distance
     * greater than the radius, it is outside the boundary. If smaller inside
     * the boundary
     *
     * @param[in] zone struct containing the zone information
     *
     * @return true if outside boundary, false if not
     */
    bool IsCircularGeofenceOutside(ZoneInfo& zone);

    /**
     * @brief Checks to see if polygonal geofence is outside the polygon
     * boundary
     *
     * @details Calls IsPointInPolygon() and if it returns true the point is
     * inside the boundary. If it returns false the point is outside the
     * boundary
     *
     * @param[in] zone struct containg the zone info
     *
     * @return true if outside the boundary, false if not
     */
    bool IsPolygonalGeofenceOutside(ZoneInfo& zone);

    /**
     * @brief Uses the even-odd rule using the ray casting method from a point
     * to see if it is inside of the polygon. Accounts for the polygon crossing
     * the internation date line. However does not account for a polygon
     * region that covers the north and/or south poles
     *
     * @details Checks if sequential polygon vertices are in the same plane as
     * a given point. If so we figure out if the segment line of two of the
     * vertices is to the right of the given point. We then flip the odd_nodes flag
     * If there are an odd number of nodes it is inside the polygon. If there
     * are an even number of nodes it is outside
     *
     * @param[in] poly_point vector containing the vertices of the polygon
     * @param[in] point_lat latitude of the given point
     * @param[in] point_lon longitude of the given point
     *
     * @return true if inside the polygon, false if outside the polygon
     */
    bool IsPointInPolygon(Vector<PolygonPoint>& poly_point,
                    double point_lat,
                    double point_lon);

    /**
     * @brief Check to see how many Polygon Points are enabled
     *
     * @details Checks the polygon points vector and counts the number of
     * enabled points
     *
     * @param[in] poly_points vector containg the vertices of the polygon
     *
     * @return number of enabled points
     */
    int HowManyPolygonPointsEnabled(Vector<PolygonPoint>& poly_points);

    /**
     * @brief Check if the zone has passed the verification_sec threshold to
     * trigger an event
     *
     * @details Checks the GeofenceZoneState using the given zone_index to see
     * if the pending event is stable and the pending time has exceeded the
     * verification_sec. If not reset the pending event to the current state
     * (inside or outside) for the next call to this function to evaluate again
     *
     * @param[in] outside_geofence currently inside or outside the geofence
     * @param[in] zone the given zone info we want to process
     * @param[in] zone_index the given zone index to be used to look up the
     * correct GeofenceZoneState.
     *
     * @return true if triggered, false if not
     */
    bool IsEventTriggered(bool outside_geofence,
                        ZoneInfo& zone,
                        int zone_index);

    /**
     * @brief If the polygon crosses the internation date line you must
     * add 360 degrees to all longitude points. This returns an offset of 360
     *
     * @details This calculates the distance covered by min longitude, and max
     * longitude of all the poly_points. The absolute value of the difference
     * abs(min_lon-max_lon) is compared to 180. If geater than 180 the polygon
     * has to have crossed the international dateline.
     *
     * @param[in,out] poly_points vector containing the vertices of the polygon
     *
     * @return 360 if crosses the international date line, or 0 if not
     */
    double CalculateLonDatelineOffset(Vector<PolygonPoint>& poly_points);

    /**
     * \brief           Calculate distance and bearing between `2` latitude and longitude coordinates
     * \param[in]       las: Latitude start coordinate, in units of degrees
     * \param[in]       los: Longitude start coordinate, in units of degrees
     * \param[in]       lae: Latitude end coordinate, in units of degrees
     * \param[in]       loe: Longitude end coordinate, in units of degrees
     * \param[out]      d: reference to output distance in units of meters
     * \return          `1` on success, `0` otherwise
     */
    inline void GpsDistance(double las,
                            double los,
                            double lae,
                            double loe,
                            double& d);
    /**
     * @brief Convert value from degrees to radians
     *
     * @details converts values from degrees to radians using a constant
     *
     * @param[in] x value in degrees
     *
     * @return value in radians
     */
    inline double D2R(double x) {return ((x) * (0.01745329251994));}

    Vector<ZoneInfo> GeofenceZones;
    Vector<GeofenceZoneState> GeofenceZoneStates;
    Vector<GeofenceEventCallback> EventCallback;

    PointData _geofence_point;
    double _maximumDop;
};
