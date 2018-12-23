#ifndef __TEMPERATURE__H__
#define __TEMPERATURE__H__


/**
 * Get a temperature sensor address
 * @return True if an address was found, False if no more address
 */
bool getAddress();

/**
 * Launch a measure on a sensor
 * @return Temperature in celcius
 */
float performMeasure();

int performRawMeasure();

/**
 * True if at least one sensor was found
 * @return True if found
 */
bool sensorFound();

#endif
