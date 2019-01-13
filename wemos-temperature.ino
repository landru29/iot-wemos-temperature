#include <ESP8266WiFi.h>

#include "temperature.h"

#include <SFE_MicroOLED.h> 

#define PIN_RESET 255
#define DC_JUMPER 0 

MicroOLED oled(PIN_RESET, DC_JUMPER);
// D1 and D2 are used by the led matrix




void showCursor(char* str) {
  oled.setCursor(56,0);
  oled.print(str);
  oled.display();
}

/**
 * Wait before launchin the next measure. It animate a curor on the screen
 */
void wait() {
  showCursor("/");
  delay(1000);
  
  showCursor("-");
  delay(1000);

  showCursor("\\");
  delay(1000);

  showCursor("|");
  delay(1000);

  showCursor("?");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  delay(3000);

  oled.begin();
  oled.clear(ALL);  
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(1);

  oled.setFontType(0);
  delay(1000);
  oled.clear(PAGE);
  oled.display();
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
      showCursor(">");

    } else {
      oled.print(" xx.xx ");
    }
    
    oled.display();
    wait();
}
