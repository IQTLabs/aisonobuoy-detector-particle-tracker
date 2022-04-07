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

#include "Particle.h"
#include "Geofence.h"


Geofence test(2);
ZoneInfo GoldenGatePark;
ZoneInfo PoloField;

SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, {
});

constexpr PointData TestPoints[] = {
    { -37.76887, 122.48248, 0.0, 0.0, 0 },   // Somewhere south of Australia
    { -34.60845, -58.37225, 0.0, 0.0, 0 },   // Buenos Aires, Argentina
    { 20.67622, -103.34693, 0.0, 0.0, 0 },   // Guadalajara, Mexico
    { 37.68821, -122.47201, 0.0, 0.0, 0 },   // Daly City In-N-Out
    { 37.74316, -122.47725, 0.0, 0.0, 0 },   // Mr Bread Bakery on Taraval
    { 37.76298, -122.45638, 0.0, 0.0, 0 },   // UCSF
    { 37.76705, -122.48593, 0.0, 0.0, 0 },   // Elk Glen Picnic Area
    { 6.721186, -179.28955, 0.0, 0.0, 0 },   // Near dateline in Pacific
    { -3.072765, -59.99389, 0.0, 0.0, 0 },   // Manuas Brazil
    { 37.75402, -122.44960, 0.0, 0.0, 0 },   // USPS Mailbox Twin Peaks
    { -0.440480, -64.598314, 0.0, 0.0, 0 },  // Papera Brazil
};

void geofenceCallback(CallbackContext& context) {
    switch(context.event_type) {
        case GeofenceEventType::ENTER:
            Log.info("Enter Geofence");
        break;
        case GeofenceEventType::EXIT:
            Log.info("Exit Geofence");
        break;
        case GeofenceEventType::OUTSIDE:
            Log.info("Outside Geofence");
        break;
        case GeofenceEventType::INSIDE:
            Log.info("Inside Geofence");
        break;
        default:
        break;
    }
}

void setup() {
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;

    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).radius = 2700.0;
    test.GetZoneInfo(1).center_lat = 37.76825;
    test.GetZoneInfo(1).center_lon = -122.49245;
    test.GetZoneInfo(1).inside_event = true;
    test.GetZoneInfo(1).shape_type = GeofenceShapeType::CIRCULAR;

    test.RegisterGeofenceCallback(geofenceCallback);
}

void loop() {
    static uint32_t update_loop_sec = 0;
    static uint32_t update_point_sec = 0;
    static unsigned int counter = 0;

    //run the geofence check once a second
    if((System.uptime() - update_loop_sec) >= 1) {
        test.loop();
        update_loop_sec = System.uptime();
    }

    //Simulate test point being inside the geofence, and outside the geofence
    //every 10 seconds
    if ((System.uptime() - update_point_sec) >= 10) {
        if(!(counter % 2)) {
            test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
        }
        else {
            test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
        }
        counter++;
        update_point_sec = System.uptime();
    }
}
