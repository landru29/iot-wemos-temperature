
#ifndef __TIMESTAMP__H__
#define __TIMESTAMP__H__

#include <SFE_MicroOLED.h> 

/**
 * Get current timestamp
 * @return timestamp
 */
long getTimestamp();

/**
 * Initialize the timer
 */
void initTimestampTimer(const char* host, const int port, const char* url, const char* fingerprint, MicroOLED* output);

#endif


