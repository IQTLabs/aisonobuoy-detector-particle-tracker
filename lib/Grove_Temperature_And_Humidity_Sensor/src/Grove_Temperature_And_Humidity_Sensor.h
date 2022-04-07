/* Seeed_DHT11 library
 *
 * MIT license
 *
 * */

#ifndef Grove_Temperature_And_Humidity_Sensor
#define Grove_Temperature_And_Humidity_Sensor

#include "Arduino.h"
#include "math.h"

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85



typedef unsigned char  u8;
typedef signed char    s8;
typedef unsigned short u16;
typedef signed short   s16;
typedef unsigned long  u32;

class DHT {
	private:
		u8 data[6];
		u8 _pin, _type, _count;
		u32   _lastreadtime;
		boolean firstreading;
		float readTemperature();
		float convertFtoC(float f);
		float convertCtoF(float c);
		float readHumidity();
		boolean read();

	public:
		DHT(u8 pin, u8 count=6);
		void  begin();
		float getHumidity();
		float getTempCelcius();
		float getTempFarenheit();
};
#endif
