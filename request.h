#ifndef __REQUEST__H__
#define __REQUEST__H__

#include <SFE_MicroOLED.h> 

/**
 * Create Authorization header field
 * @param user     User string
 * @param password Password String
 * @return "Basic " + base64
 */
String createAuthorization(const char* user, const char* password);


/**
 * Send Http request
 * @param methodName GET, POST, PUT or DELETE
 * @param host       hostname
 * @param port       Port (443)
 * @param body       Body content
 * @param user       User for authentication
 * @param password   Password for authentication
 * @param oled       Output led matrix
 * @return Response
 */
String sendRequest(const char* methodName, const char* host, const int port, const char* url, const char* fingerprint, String body, const char* user, const char* password, MicroOLED output);

#endif
