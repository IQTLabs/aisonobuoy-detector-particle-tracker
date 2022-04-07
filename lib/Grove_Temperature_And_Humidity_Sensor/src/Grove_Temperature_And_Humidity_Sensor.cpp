/* Seeed_DHT11 library
 *
 * MIT license
 *
 * */

#include "Grove_Temperature_And_Humidity_Sensor.h"

DHT::DHT(u8 pin, u8 count)
{
	_pin = pin;
	_count = count;
	firstreading = true;
}

void DHT::begin(void)
{
// set up the pins!
	pinMode(_pin, INPUT);
	pinSetFast(_pin);
	_lastreadtime = 0;
}

float DHT::readTemperature()
{
	float f;

	if (read())
	{
		f = data[2];
		return f;
	}
	return NAN;
}

float DHT::getHumidity()
{
	return readHumidity();
}

float DHT::getTempCelcius()
{
	return readTemperature();
}

float DHT::getTempFarenheit()
{
	return convertCtoF(readTemperature());
}


float DHT::convertFtoC(float f)
{
	return (f - 32) * 5 / 9;
}

float DHT::convertCtoF(float c)
{
	return c * 9 / 5 + 32;
}

float DHT::readHumidity(void)
{
	float f;

	if (read())
	{
		f = data[0];
		return f;
	}
	return NAN;
}


boolean DHT::read(void)
{
	u8 laststate = HIGH;
	u8 counter = 0;
	u8 j = 0, i;
	u32 currenttime;

// Check if sensor was read less than two seconds ago and return early
// to use last reading.
	currenttime = millis();
	if (currenttime < _lastreadtime)
	{
// ie there was a rollover
		_lastreadtime = 0;
	}
	if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
		return true; // return last correct measurement
//		delay(2000 - (currenttime - _lastreadtime));
	}
	firstreading = false;
/*
	Serial.print("Currtime: "); Serial.print(currenttime);
	Serial.print(" Lasttime: "); Serial.print(_lastreadtime);
*/
	_lastreadtime = millis();

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

// pull the pin high and wait 250 milliseconds
	pinSetFast(_pin);
	delay(250);

// send begin signal
	pinMode(_pin, OUTPUT);
	pinResetFast(_pin);
	delay(20);
	noInterrupts();
	pinSetFast(_pin);
	delayMicroseconds(40);
	pinMode(_pin, INPUT);

// read high/low status
	for ( i=0; i< MAXTIMINGS; i++) {
		counter = 0;
		while (pinReadFast(_pin) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = pinReadFast(_pin);

		if (counter == 255) break;

// ignore first 3 transitions,it's response signal
		if ((i >= 4) && (i%2 == 0)) {
// shove each bit into the storage bytes
			data[j/8] <<= 1;
			if (counter > _count)
				data[j/8] |= 1;
			j++;
		}

	}

	interrupts();

/*
	Serial.println(j, DEC);
	Serial.print(data[0], HEX); Serial.print(", ");
	Serial.print(data[1], HEX); Serial.print(", ");
	Serial.print(data[2], HEX); Serial.print(", ");
	Serial.print(data[3], HEX); Serial.print(", ");
	Serial.print(data[4], HEX); Serial.print(" =? ");
	Serial.println(data[0] + data[1] + data[2] + data[3], HEX);
*/

// Verify that the data is correct
	if ((j >= 40) &&
	   (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
		return true;
	}

	return false;

}
