
#include "timestamp.h"
#include <user_interface.h>
#include <Arduino.h>
#include "request.h"

long currentTimestamp = 0;
os_timer_t myTimer;

char* tHost;
int tPort;
char* tUrl;
char* tFingerprint;
MicroOLED* tOutput;

/**
 * Invoked by timer interrupt
 */
void timerCallback(void *pArg) {
  if (currentTimestamp > 10000) {
    currentTimestamp++;
  }
}

/**
 * Get current timestamp
 * @return timestamp
 */
long getTimestamp() {
  if (currentTimestamp<100000) {
    String timestamp = sendRequest("GET", tHost, tPort, tUrl, tFingerprint, "", 0, 0, *tOutput);
    currentTimestamp = timestamp.toInt();
  }
  return currentTimestamp;
}

/**
 * Initialize the timer
 */
void initTimestampTimer(const char* host, const int port, const char* url, const char* fingerprint, MicroOLED* output) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);
  tHost = (char*)host;
  tPort = port;
  tUrl = (char*)url;
  tFingerprint = (char*)fingerprint;
  tOutput = output;
}
