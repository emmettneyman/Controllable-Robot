//--------------------- Includes ---------------------------//
#include "Arduino.h"
#include "drive.h"

//--------------------- Variables ---------------------------//

double filter = 0.7;
byte x = 0;
byte y = 0;
byte r = 0;
const uint8_t pinx = 1;
const uint8_t piny = 2;
const uint8_t pinr = 3;
long oldtime = 0;

//---------------- Setup ---------------------------------//
void setup() {   // Setup SPI lines for the switch
  drive.SETUP();
  drive.CALCULATE(0, 0, 0);
  Serial.begin(9600);
  delay(1000);
}

//-------------- Main ---------------------------------//
void loop() {
  if (millis() - oldtime > 50) {
    x = (analogRead(pinx) / 4) * (1 - filter) + x * filter;
    y = (analogRead(piny) / 4) * (1 - filter) + y * filter;
    r = (analogRead(pinr) / 4) * (1 - filter) + r * filter;

    drive.CALCULATE(x - 128, y - 128, r - 128);
    oldtime = millis();
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.println(r);
  }
  drive.DRIVE();
}
