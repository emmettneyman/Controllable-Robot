//--------------------- Includes ---------------------------//
#include "Arduino.h"
#include "drive.h"

//--------------------- Variables ---------------------------//

byte incomingByte = 0;
byte x = 128;
byte y = 128;
byte r = 128;
const uint8_t pinx = 5;
const uint8_t piny = 6;
const uint8_t pinr = 13;

//--------------------- Software Serial Setup ---------------------------//

#include <SoftwareSerial.h>

const int IMP_SERIAL_RX = 8;
const int IMP_SERIAL_TX = 9;

// Create an instance of software serial.
SoftwareSerial impSerial(IMP_SERIAL_RX, IMP_SERIAL_TX);

//---------------- Setup ---------------------------------//
void setup() {   // Setup SPI lines for the switch
  drive.SETUP();
  drive.CALCULATE(0, 0, 0);
  pinMode(pinx, OUTPUT);
  pinMode(piny, OUTPUT);
  pinMode(pinr, OUTPUT);
  impSerial.begin(19200);
  Serial.begin(9600);
  delay(1000);
}

//-------------- Main ---------------------------------//
void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.peek();

    if (incomingByte == 'x' || incomingByte == 'X') {
      x = Serial.parseInt();
    } else if (incomingByte == 'y' || incomingByte == 'Y') {
      y = Serial.parseInt();
    } else if (incomingByte == 'r' || incomingByte == 'R') {
      r = Serial.parseInt();
    } else {
      Serial.read();
    }
    drive.CALCULATE(x - 128, y - 128, r - 128);

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" R: ");
    Serial.println(r);
    
  }

  if (impSerial.available() > 0) {
    // read the incoming byte:
    incomingByte = impSerial.peek();

    if (incomingByte == 'x' || incomingByte == 'X') {
      x = impSerial.parseInt();
    } else if (incomingByte == 'y' || incomingByte == 'Y') {
      y = impSerial.parseInt();
    } else if (incomingByte == 'r' || incomingByte == 'R') {
      r = impSerial.parseInt();
    } else {
      impSerial.read();
    }
    drive.CALCULATE(x - 128, y - 128, r - 128);

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" R: ");
    Serial.println(r);
    
  }



  analogWrite(pinx, x);
  analogWrite(piny, y);
  analogWrite(pinr, r);

  drive.DRIVE();
}


