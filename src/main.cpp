#include "Particle.h"

#include "tracker_config.h"
#include "tracker.h"

#include "Grove_Temperature_And_Humidity_Sensor.h"
#include "TemperatureHumidityValidatorRK.h"

// fix pgmspace issue https://github.com/adafruit/Adafruit_nRF52_Arduino/pull/503
// C:\Users\rcaud\.particle\toolchains\deviceOS\3.3.0-rc.1\wiring\inc\avr\pgmspace.h
#include <ArduinoJson.h>
#include <Wire.h>

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

PRODUCT_ID(TRACKER_PRODUCT_ID);
PRODUCT_VERSION(TRACKER_PRODUCT_VERSION);

SerialLogHandler logHandler(115200, LOG_LEVEL_TRACE, {
    { "app.gps.nmea", LOG_LEVEL_INFO },
    { "app.gps.ubx",  LOG_LEVEL_INFO },
    { "ncp.at", LOG_LEVEL_INFO },
    { "net.ppp.client", LOG_LEVEL_INFO },
});

// Library: Grove_Temperature_And_Humidity_Sensor
DHT tempSensor(A3);

// Library: TemperatureHumidityValidatorRK
TemperatureHumidityValidator validator;

// Sample the temperature sensor every 2 seconds. This is done so the outlier values can be filtered out easily.
const unsigned long CHECK_PERIOD_MS = 2000;
unsigned long lastCheck = 0;

void locationGenerationCallback(JSONWriter &writer, LocationPoint &point, const void *context); // Forward declaration
void recvHandler(int _i); // Forward declaration

String inputString = ""; // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

const byte bufsize = 128;
const byte i2caddr = 4;

uint16_t detected = 0;
DynamicJsonDocument doc(bufsize);
char buf[bufsize] = {0};

void setup()
{
    Tracker::instance().init();
    
    // Callback to add key press information to the location publish
    Tracker::instance().location.regLocGenCallback(locationGenerationCallback);

    // Initialize temperature sensor
    tempSensor.begin();

    // Myriota LEUART init
    Serial1.begin(9600);

    // i2c sensor init
    Wire.begin(i2caddr);
    Wire.onReceive(recvHandler);

    Particle.connect();
}

void loop()
{
    Tracker::instance().loop();

    if (millis() - lastCheck >= CHECK_PERIOD_MS) {
        lastCheck = millis();
        validator.addSample(tempSensor.getTempCelcius(), tempSensor.getHumidity());
        // Log.info("tempC=%f tempF=%f humidity=%f", validator.getTemperatureC(), validator.getTemperatureF(), validator.getHumidity());
    }

    while (Serial1.available()) {
        // get the new byte:
        char inChar = (char)Serial1.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n') {
        stringComplete = true;
        }
    }

    // print the string when a newline arrives:
    if (stringComplete) {
        Serial.print("Received data from Myriota modem: ");
        Serial.println(inputString);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }

}

// data is send using the location callback. This allows us to use the Particle "Asset Tracker Settings" configuration (i.e. sleep) control
void locationGenerationCallback(JSONWriter &writer, LocationPoint &point, const void *context)
{
    // check and add temperature data to json buffer
    float tempF = validator.getTemperatureF();
    if (!isnan(tempF)) {
        writer.name("temp").value(tempF, 2);
    }

    // check and add humidity data to json buffer
    float hum = validator.getHumidity();
    if (!isnan(hum)) {
        writer.name("hum").value(hum, 1);
    }

    // add sensor events to json buffer
    writer.name("detected").value(detected);
    
    // convert 'detected' to byte array and write to Myriota modem
    unsigned char bytes[2];
    bytes[0] = (detected >> 8) & 0xFF;
    bytes[1] = detected & 0xFF;

    // Serial.print("Sending: ");
    // Serial.println(detected);
    // Serial.print("bytes[0]: ");
    // Serial.println(bytes[0]);
    // Serial.print("bytes[1]: ");
    // Serial.println(bytes[1]);
    
    Serial1.write(bytes[0]);
    Serial1.write(bytes[1]);
}

void recvHandler(int _i) {
  char *p = buf;
  while (Wire.available()) {
    *(p++) = Wire.read();
  }
  DeserializationError error = deserializeJson(doc, buf, sizeof(buf));
  if (!error) {
    // Continuously increment, so as not to lose detections if an individual update fails.
    // int will eventually rollover, which the external infrastructure must handle.
    int new_detected = doc["boats_detected"];
    detected = new_detected;
  } else {
      Serial.println("recvHandler error");
  }
}
