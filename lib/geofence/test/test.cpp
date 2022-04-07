#include <atomic>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Geofence.h"

ZoneInfo GoldenGatePark;
ZoneInfo PoloField;
ZoneInfo EquitationField;
ZoneInfo StrawberryHill;
ZoneInfo InternationalDateline;
ZoneInfo PacificEquator;
ZoneInfo BrazilArea;
ZoneInfo TwinPeaksArea;

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

std::atomic<int> badCount(0);
std::atomic<int> enterCount(0);
std::atomic<int> exitCount(0);
std::atomic<int> outsideCount(0);
std::atomic<int> insideCount(0);
void geofenceCallback(CallbackContext& context) {
    switch(context.event_type) {
        case GeofenceEventType::POOR_LOCATION:
            badCount++;
        break;
        case GeofenceEventType::ENTER:
            enterCount++;
        break;
        case GeofenceEventType::EXIT:
            exitCount++;
        break;
        case GeofenceEventType::OUTSIDE:
            outsideCount++;
        break;
        case GeofenceEventType::INSIDE:
            insideCount++;
        break;
        default:
        break;
    }
}

TEST_CASE("Check Defaults Test") {
    Geofence test(4);
    test.init();

    auto zone0 = test.GetZoneInfo(0);
    REQUIRE(zone0.radius == 0.0);
    REQUIRE(zone0.center_lat == 0.0);
    REQUIRE(zone0.center_lon == 0.0);
    REQUIRE(zone0.enable == false);
    REQUIRE(zone0.enter_event == false);
    REQUIRE(zone0.exit_event == false);
    REQUIRE(zone0.inside_event == false);
    REQUIRE(zone0.outside_event == false);
    REQUIRE(zone0.verification_time_sec == 0);

    auto zone1 = test.GetZoneInfo(1);
    REQUIRE(zone1.radius == 0.0);
    REQUIRE(zone1.center_lat == 0.0);
    REQUIRE(zone1.center_lon == 0.0);
    REQUIRE(zone1.enable == false);
    REQUIRE(zone1.enter_event == false);
    REQUIRE(zone1.exit_event == false);
    REQUIRE(zone1.inside_event == false);
    REQUIRE(zone1.outside_event == false);
    REQUIRE(zone1.verification_time_sec == 0);

    auto zone2 = test.GetZoneInfo(2);
    REQUIRE(zone2.radius == 0.0);
    REQUIRE(zone2.center_lat == 0.0);
    REQUIRE(zone2.center_lon == 0.0);
    REQUIRE(zone2.enable == false);
    REQUIRE(zone2.enter_event == false);
    REQUIRE(zone2.exit_event == false);
    REQUIRE(zone2.inside_event == false);
    REQUIRE(zone2.outside_event == false);
    REQUIRE(zone2.verification_time_sec == 0);

    auto zone3 = test.GetZoneInfo(3);
    REQUIRE(zone3.radius == 0.0);
    REQUIRE(zone3.center_lat == 0.0);
    REQUIRE(zone3.center_lon == 0.0);
    REQUIRE(zone3.enable == false);
    REQUIRE(zone3.enter_event == false);
    REQUIRE(zone3.exit_event == false);
    REQUIRE(zone3.inside_event == false);
    REQUIRE(zone3.outside_event == false);
    REQUIRE(zone3.verification_time_sec == 0);
}

TEST_CASE("Disabled Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.SetZoneInfo(1, PoloField);
    test.SetZoneInfo(2, EquitationField);
    test.SetZoneInfo(3, StrawberryHill);

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[2]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[3]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Circular Outside Event Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).outside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).outside_event = true;
    test.GetZoneInfo(1).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).outside_event = true;
    test.GetZoneInfo(2).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).outside_event = true;
    test.GetZoneInfo(3).shape_type = GeofenceShapeType::CIRCULAR;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[1]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[2]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);

    test.UpdateGeofencePoint(TestPoints[3]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 4);
}

TEST_CASE("Circular Inside Event Test") {
    Geofence test(4);
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
    test.GetZoneInfo(1).inside_event = true;
    test.GetZoneInfo(1).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).inside_event = true;
    test.GetZoneInfo(2).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).inside_event = true;
    test.GetZoneInfo(3).shape_type = GeofenceShapeType::CIRCULAR;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Circular Enter Event Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).enter_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).inside_event = true;
    test.GetZoneInfo(1).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).inside_event = true;
    test.GetZoneInfo(2).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).inside_event = true;
    test.GetZoneInfo(3).shape_type = GeofenceShapeType::CIRCULAR;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Circular Exit Event Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).exit_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(1, PoloField);
    test.GetZoneInfo(1).enable = true;
    test.GetZoneInfo(1).inside_event = true;
    test.GetZoneInfo(1).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(2, EquitationField);
    test.GetZoneInfo(2).enable = true;
    test.GetZoneInfo(2).inside_event = true;
    test.GetZoneInfo(2).shape_type = GeofenceShapeType::CIRCULAR;
    test.SetZoneInfo(3, StrawberryHill);
    test.GetZoneInfo(3).enable = true;
    test.GetZoneInfo(3).inside_event = true;
    test.GetZoneInfo(3).shape_type = GeofenceShapeType::CIRCULAR;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    //outside the zone
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Multiple Circular Zone Events Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).enter_event = true;
    test.GetZoneInfo(0).exit_event = true;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).outside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    //outside the zone
    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[0]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);
}

TEST_CASE("Verification Time Circular Zone Events Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).enter_event = true;
    test.GetZoneInfo(0).exit_event = true;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).outside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;
    test.GetZoneInfo(0).verification_time_sec = 3; //3 seconds

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    //outside the zone
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();

    //outside the zone
    System.inc(2000);
    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    //outside the zone
    System.inc(3000);
    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    //outside the zone
    System.inc(1000);
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    //inside the zone
    test.UpdateGeofencePoint(TestPoints[6]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    //inside the zone
    System.inc(2000);
    test.UpdateGeofencePoint(TestPoints[6]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

     //inside the zone
    System.inc(3000);
    test.UpdateGeofencePoint(TestPoints[6]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    //inside the zone
    System.inc(1000);
    test.UpdateGeofencePoint(TestPoints[6]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    //outside the zone
    System.inc(3000);
    test.UpdateGeofencePoint(TestPoints[1]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    //outside the zone
    System.inc(1000);
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    //outside the zone
    System.inc(2000);
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    //outside the zone
    System.inc(1000);
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    //outside the zone
    System.inc(2000);
    test.UpdateGeofencePoint(TestPoints[0]);
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);
}

TEST_CASE("Polygonal Inside Event Test") {

    Vector<PolygonPoint> gg_park_polygon{{37.771531,-122.511040,true},
        {37.764150,-122.510452,true},{37.766465,-122.453018,true},
        {37.774911,-122.454279,true}};
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 0.0;
    test.GetZoneInfo(0).center_lat = 0.0;
    test.GetZoneInfo(0).center_lon = 0.0;
    test.GetZoneInfo(0).polygon_points = gg_park_polygon;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::POLYGONAL;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[3]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[6]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("International Dateline Polygonal Inside Event Test") {

    Vector<PolygonPoint> int_dateline_polygon{{7.870459,175.459385,true},
        {4.504215,175.459385,true},{4.790698,-176.611013,true},
        {10.058798,-176.248620,true}};
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, InternationalDateline);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 0.0;
    test.GetZoneInfo(0).center_lat = 0.0;
    test.GetZoneInfo(0).center_lon = 0.0;
    test.GetZoneInfo(0).polygon_points = int_dateline_polygon;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::POLYGONAL;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[3]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[7]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Southern hemisphere Polygonal Inside Event Test") {

    Vector<PolygonPoint> south_hemi_polygon{{-2.992267,-60.130649,true},
        {-3.152658,-60.124700,true},{-3.155628,-59.919461,true},
        {-3.027912,-59.901614,true}};
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, BrazilArea);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 0.0;
    test.GetZoneInfo(0).center_lat = 0.0;
    test.GetZoneInfo(0).center_lon = 0.0;
    test.GetZoneInfo(0).polygon_points = south_hemi_polygon;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::POLYGONAL;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[3]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[8]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Brazil Equator Crossing Polygon Inside Event Test") {

    Vector<PolygonPoint> south_hemi_polygon{{0.287359,-65.374218,true},
        {-0.762855,-65.382897,true},{-0.635478,-64.320909,true},
        {0.265387,-64.307176,true}};
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, BrazilArea);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 0.0;
    test.GetZoneInfo(0).center_lat = 0.0;
    test.GetZoneInfo(0).center_lon = 0.0;
    test.GetZoneInfo(0).polygon_points = south_hemi_polygon;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::POLYGONAL;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[3]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[4]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[5]); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(TestPoints[10]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}

TEST_CASE("Disabled Point Polygon Inside Event Test") {
    Vector<PolygonPoint> qauadrilateral{{37.74911,-122.45690,true},
        {37.75149,-122.44779,true},{37.75494,-122.44662,true},
        {37.75524,-122.45275,true}};
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, TwinPeaksArea);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 0.0;
    test.GetZoneInfo(0).center_lat = 0.0;
    test.GetZoneInfo(0).center_lon = 0.0;
    test.GetZoneInfo(0).polygon_points = qauadrilateral;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::POLYGONAL;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    test.UpdateGeofencePoint(TestPoints[9]); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    //makes a triangle now, which puts the point to the outside of geofence
    test.GetZoneInfo(0).polygon_points.at(2).enable = false;
    test.GetZoneInfo(0).outside_event = true;

    test.UpdateGeofencePoint(TestPoints[9]); //outside the zone now since removed the point
    test.loop();
    REQUIRE(badCount.exchange(0) == 0);
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);
}

TEST_CASE("Confidence Level Test") {
    Geofence test(4);
    test.init();

    test.SetZoneInfo(0, GoldenGatePark);
    test.GetZoneInfo(0).enable = true;
    test.GetZoneInfo(0).radius = 2700.0;
    test.GetZoneInfo(0).center_lat = 37.76887;
    test.GetZoneInfo(0).center_lon = -122.48248;
    test.GetZoneInfo(0).enter_event = true;
    test.GetZoneInfo(0).exit_event = true;
    test.GetZoneInfo(0).inside_event = true;
    test.GetZoneInfo(0).outside_event = true;
    test.GetZoneInfo(0).shape_type = GeofenceShapeType::CIRCULAR;

    // UCSF, inside of the 2.7km fence
    PointData ucsf = TestPoints[5];
    ucsf.hdop = 2.0;
    ucsf.horizontal_accuracy = 10.0;

    // Mr Bread Bakery on Taraval, immediately outside of the 2.7km fence
    PointData mrbread = TestPoints[4];
    mrbread.hdop = 2.0;
    mrbread.horizontal_accuracy = 10.0;

    REQUIRE(test.RegisterGeofenceCallback(geofenceCallback) == SYSTEM_ERROR_NONE);

    // Set the system to use a mavimum of 5.0 DOP
    test.SetMaximumHdopLevel(5.0);

    // Test the normal case where the HDOP is under the max DOP limit
    test.UpdateGeofencePoint(ucsf); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0); // Not considered a poor location
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    test.UpdateGeofencePoint(mrbread); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0); // Not considered a poor location
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    // Go back inside of the zone and try updating with a poor outside point
    test.UpdateGeofencePoint(ucsf); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0); // Not considered a poor location
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);

    mrbread.hdop = 6.0; // worse than the 5.0 limit
    test.UpdateGeofencePoint(mrbread); //outside the zone but should be ignored
    test.loop();
    // The poor location count should have been indicated
    REQUIRE(badCount.exchange(0) == 1);
    // No counter change means there are no geofences evaluated
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 0);

    // Now improve the HDOP figure
    mrbread.hdop = 4.0; // better than the 5.0 limit
    test.UpdateGeofencePoint(mrbread); //outside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0); // Not considered a poor location
    REQUIRE(enterCount.exchange(0) == 0); REQUIRE(exitCount.exchange(0) == 1); REQUIRE(insideCount.exchange(0) == 0); REQUIRE(outsideCount.exchange(0) == 1);

    // Back inside the zone
    test.UpdateGeofencePoint(ucsf); //inside the zone
    test.loop();
    REQUIRE(badCount.exchange(0) == 0); // Not considered a poor location
    REQUIRE(enterCount.exchange(0) == 1); REQUIRE(exitCount.exchange(0) == 0); REQUIRE(insideCount.exchange(0) == 1); REQUIRE(outsideCount.exchange(0) == 0);
}
