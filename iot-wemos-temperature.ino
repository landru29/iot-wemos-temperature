#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "config.h"

#ifndef __CONFIG_IOT_TEMP__H__
    #error You must first create config.h based on config.h.sample"
#endif

#include "request.h"
#include "temperature.h"

#include <SFE_MicroOLED.h> 

#define PIN_RESET 255
#define DC_JUMPER 0 

MicroOLED oled(PIN_RESET, DC_JUMPER);
// D1 and D2 are used by the led matrix
long currentTimestamp = 0;

const char* iotHost = "opentsdb.iot.runabove.io";
const char* iotUrl = "/api/put";
const int iotPort = 443;

const char* apiHost = "api.ovh.com";
const char* apiUrl = "/1.0/auth/time";
const int apiPort = 443;

// Use https://www.grc.com/fingerprints.htm
// SHA1 fingerprint of the certificate
const char* iotFingerprint = "50 D2 67 15 6C 86 28 43 6E 27 F1 C7 9F 60 7D E3 BE 60 9A 9D";
const char* apiFingerprint = "B2 B5 11 E3 EA 5D 3A C0 72 E0 7E 70 46 C8 D1 CA EA 88 26 A9";

/**
 * Build IOT data
 * @param name Metric name
 * @param value  Metric value
 * @return JSON data
 */
String buildData(String timestamp, const char* name, float value) {
  return String("[{\"metric\":\"") + String(name) + String("\",\"value\":" + String(value) + ",\"timestamp\":" + timestamp + "}]");
}

/**
 * Connect to the wifi
 * @param wifi_ssid     Wifi SSID
 * @param wifi_password Wifi password
 */
void wifiConnect(const char* wifi_ssid, const char* wifi_password) {
  Serial.print("connecting to ");
  Serial.println(wifi_ssid);
  oled.setCursor(0,0);
  oled.print("Wifi ");
  oled.display();
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    oled.print(".");
    oled.display();
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  oled.println("");
  oled.println(WiFi.localIP());
  oled.display();
}

/**
 * Wait before launchin the next measure. It animate a curor on the screen
 */
void wait() {
  oled.setCursor(56,0);
  oled.print("/");
  oled.display();
  delay(1000);
  
  oled.setCursor(56,0);
  oled.print("-");
  oled.display();
  delay(1000);

  oled.setCursor(56,0);
  oled.print("\\");
  oled.display();
  delay(1000);

  oled.setCursor(56,0);
  oled.print("|");
  oled.display();
}

void setup() {
  Serial.begin(115200);

  oled.begin();
  oled.clear(ALL);  
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(0);

  wifiConnect(ssid, password);
  oled.setFontType(1);
  delay(1000);
  oled.clear(PAGE);
  oled.display();
}

long getTime() {
  //if (currentTimestamp<100000) {
    String timestamp = sendRequest("GET", apiHost, apiPort, apiUrl, apiFingerprint, "", 0, 0, oled);
    currentTimestamp = timestamp.toInt();
  //}
  return currentTimestamp;
}

/**
 * Main loop
 */
void loop() {
   
   oled.setCursor(0,0);
   oled.println("Temp:");
   oled.display();
   if (getAddress()) {
      float celcius = performMeasure();
      Serial.println(celcius);
      oled.print(" ");
      oled.println(celcius);
      oled.print("celcius");
      
      sendRequest("POST", iotHost, iotPort, iotUrl, iotFingerprint, buildData(String(getTime()), "Temperature", celcius), iotId, iotKey, oled);
    } else {
      oled.print(" xx.xx ");
    }
    
    oled.display();
    wait();
}
