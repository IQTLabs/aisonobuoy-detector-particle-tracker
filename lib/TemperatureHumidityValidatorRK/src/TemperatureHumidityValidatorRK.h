#ifndef __TEMPERATUREHUMIDITYVALIDATOR_RK_H
#define __TEMPERATUREHUMIDITYVALIDATOR_RK_H

#include "Particle.h"

/**
 * @brief Smooths out values from sensors like the DHT11 and DHT22
 * 
 * On Gen 3 devices in particular, these sensors can return invalid (bitshifted)
 * values but since the sensor doesn't send a CRC, there's no way to determine
 * the value is bad.
 * 
 * Create a TemperatureHumidityValidator on the stack. Query the sensor 
 * periodically, preferably once a second to several seconds and call
 * addSample with the temperature and humidity values. If you don't have
 * humidity you can omit that argument.
 * 
 * To get the temperature, use the getTemperatureC() or getTemperatureF()
 * function. 
 * 
 * For humidity, use getHumidity(). This returns relative humidity in
 * a percentage from 0.0 to 100.0.
 * 
 * Note: These functions will return nan (non-a-number) until addSample
 * is called 10 times with at least somewhat valid samples. Before that, it's 
 * hard to determine what's valid or not, and this is also why you should
 * call it periodically.
 * 
 * The returned temperature is calculated by calculating the mean of all
 * temperatures within 1 standard deviation of the mean. This removes the
 * outliers and makes the calculation more reliable.
 * 
 * The specific error that is typically encountered is a bit shift to make
 * the temperature or humidity twice as large as it normally should be.
 * This algorithm works well for filtering that out.
 */
class TemperatureHumidityValidator {
public:
    /**
     * @brief Constructs the object. 
     * 
     * You normally instantiate one of these as a global variable.
     * 
     * This object does not allocate additional heap memory, however it does
     * have 84 bytes used to buffer samples in the object.
     */
    TemperatureHumidityValidator();

    /**
     * @brief Destructor
     * 
     * You normally don't delete one of these as it's usually a global variable.
     */
    virtual ~TemperatureHumidityValidator();

    /**
     * @brief Add a sample to the validator
     * 
     * With the Grove_Temperature_And_Humidity_Sensor library, for example:
     * validator.addSample(tempSensor.getTempCelcius(), tempSensor.getHumidity());
     */
    void addSample(float temperatureC, float humidity = 0.0);

    /**
     * @brief Gets the temperature in degrees Celsius.
     * 
     * Note: Returns nan (not a number) when the temperature cannot be determined
     * because there are not a sufficient number of samples. Use isnan(value) to
     * determine if the value is nan or not.
     */
    float getTemperatureC() const;

    /**
     * @brief Gets the temperature in degrees Fahrenheit.
     * 
     * Note: Returns nan (not a number) when the temperature cannot be determined
     * because there are not a sufficient number of samples. Use isnan(value) to
     * determine if the value is nan or not.
     */
    float getTemperatureF() const;

    /**
     * @brief Gets the humidity in percentage relative humidity in the range of 0.0 to 100.0.
     * 
     * Note: Returns nan (not a number) when the humidity cannot be determined
     * because there are not a sufficient number of samples. Use isnan(value) to
     * determine if the value is nan or not.
     */
    float getHumidity() const;

    static const size_t NUM_SAMPLES = 10;

protected:
    /**
     * @brief Used internally to calculate the value from an array of samples
     * 
     * If tempSamples is passed in, it's temperature. If humSamples, it's humidity.
     * 
     * Don't call this if numSamples < NUM_SAMPLES!
     */
    static float getValue(const float *samples);

    size_t numSamples = 0;
    float tempSamples[NUM_SAMPLES];
    float humSamples[NUM_SAMPLES];

};

#endif /* __TEMPERATUREHUMIDITYVALIDATOR_RK_H */
