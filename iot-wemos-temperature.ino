#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "temperature.h"

#include <SFE_MicroOLED.h> 
#include <string.h>

#define PIN_RESET 255
#define DC_JUMPER 0 

MicroOLED oled(PIN_RESET, DC_JUMPER);
// D1 and D2 are used by the led matrix

int minutes = 0;
int seconds = 0;
int hours = 0;
char chrono[10] = "";
char temperature[10] = "";
float celcius;
int busy = 0;

//#define PRE_LOAD 313000
#define PRE_LOAD 312500


void tick () {
  char data[10];
  oled.setCursor(5,20);
  sprintf(data, "%02d:%02d:%02d", hours, minutes, seconds);
  oled.print(data);
  oled.display();
}

void ICACHE_RAM_ATTR onTimerISR(){
  if (++seconds > 59) {
    seconds = 0;
    if (++minutes > 59) {
      minutes = 0;
      hours++;
    }
  }
  
  if (!busy) {
    busy = 0;
    if (getAddress()) {
      int rawMeasure = performRawMeasure();
      int temp = rawMeasure / 16;
      int digit = rawMeasure * 100 / 16 - temp * 100;
      sprintf(temperature, "%d.%d c ", temp, digit);
    }
    
    sprintf(chrono, "%02d:%02d:%02d", hours, minutes, seconds);
    oled.setCursor(5,10);
    oled.println(temperature);
    oled.setCursor(5,25);
    oled.println(chrono);
    oled.display();
    busy = 0;
  }
}

void setup() {
  Serial.begin(115200);

  oled.begin();
  oled.clear(ALL);  
  oled.clear(PAGE);
  oled.display();
  delay(1000);

  oled.setFontType(0);
  delay(1000);
  oled.clear(PAGE);
  oled.display();
  oled.setCursor(0,0);
  oled.println("Temp:");
  oled.display();

  //Initialize Ticker every 0.5s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(3, TIM_EDGE, TIM_LOOP);
  timer1_write(PRE_LOAD); //120000 us
}

/*void getTemp() {
   oled.setCursor(0,0);
   oled.println("Temp:");
   if (getAddress()) {
      float celcius = performMeasure();
      oled.print(" ");
      oled.print(String() + celcius + String(" c"));
    } else {
      oled.print(" xx.xx ");
    }
}*/

/**
 * Main loop
 */
void loop() {
   
   //getTemp();
   //tick();
   //delay(100);
}
