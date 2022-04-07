#include <Grove_Temperature_And_Humidity_Sensor.h>

//An example of DHT11 for PHOTON

#define DHTPIN 	D2     // set pin

DHT dht(DHTPIN);

void setup() {
	Serial.begin(9600);
	Serial.println("DHT11 Begin!!!");

	dht.begin();
}

void loop() {
// Wait a few seconds between measurements.
// The sensor reads data slowly.
	delay(1000);



//Read Humidity
	float h = dht.getHumidity();
// Read temperature as Celsius
	float t = dht.getTempCelcius();
// Read temperature as Farenheit
	float f = dht.getTempFarenheit();

// Check if any reads failed
	if (isnan(h) || isnan(t) || isnan(f))
	{
		Serial.println("Failed to read from DHT11 sensor!");
		return;
	}

	Serial.print("Humid: ");
	Serial.print(h);
	Serial.println("%  ");
	Serial.print("Temp: ");
	Serial.print(t);
	Serial.println("*C ");
	Serial.print("Temp: ");
	Serial.print(f);
	Serial.println("*F ");
	Serial.println();Serial.println();
}
