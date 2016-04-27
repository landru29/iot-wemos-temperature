
#include <Arduino.h>
#include <base64.h>
#include <WiFiClientSecure.h>
#include "request.h"

/**
 * Create Authorization header field
 * @param user     User string
 * @param password Password String
 * @return "Basic " + base64
 */
String createAuthorization(const char* user, const char* password) {
  String compose = String(user) + ":" + String(password); 
  return String("Basic ") + base64::encode((uint8_t*)compose.c_str(), compose.length());
}



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
String sendRequest(const char* methodName, const char* host, const int port, const char* url, const char* fingerprint, String body, const char* user, const char* password, MicroOLED output) {
  
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("Connecting to ");
  Serial.println(host);
  if (!client.connect(host, port)) {
    Serial.println(" => Connection failed");
    return String("connection failed");
  }

  if (client.verify(fingerprint, host)) {
    Serial.println(" => Certificate matches");
  } else {
    Serial.println(" => Certificate does not match :-(");
  }

  unsigned int bodyLength = body.length();

  String requestContent = String(methodName) + " " + String(url) + 
               " HTTP/1.1\r\n" +
               "User-Agent: ESP8266/WemosD1Mini\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n" +
               "Accept: */*\r\n" +
               "Content-Type: application/json\r\n" +
               (user ? String("Authorization: " + createAuthorization(user, password) + "\r\n") : String()) +
               (bodyLength ? String("Content-Length: ") + String(bodyLength) + String("\r\n") : String("")) +
                "\r\n" +
               (bodyLength ? body + "\r\n" : String());

  Serial.print("Sending ");
  Serial.print(methodName);
  Serial.print(" to ");
  Serial.println(url);
  Serial.println("---");
  Serial.println(requestContent);
  Serial.println("---");
  client.print(requestContent);

  Serial.println("Request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  Serial.println("Response:");
  Serial.println("---");
  Serial.println(line);
  Serial.println("---");
  return line;
}
