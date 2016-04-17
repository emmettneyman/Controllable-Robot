
#include "Arduino.h"
#include "drive.h"

const double change = 0.1;

const uint8_t IN21 = 5;
const uint8_t IN22 = 6;
const uint8_t IN31 = 9;
const uint8_t IN32 = 10;
const uint8_t EN21 = 0;
const uint8_t EN22 = 1;
const uint8_t EN31 = 2;
const uint8_t EN32 = 3;

int target1, target2, target3;

volatile long encoderValue2 = 0;
volatile long encoderValue3 = 0;
volatile long lastEncoded2 = 0;
volatile long lastEncoded3 = 0;

long oldEncoder2 = 0;
long oldEncoder3 = 0;

int rpm2 = 0;
int rpm3 = 0;
double speed2 = 0;
double speed3 = 0;
int movement2;
int movement3;
unsigned long timeold2 = 0;
unsigned long timeold3 = 0;

void updateEncoder2() {
  int MSB = digitalRead(EN21); //MSB = most significant bit
  int LSB = digitalRead(EN22); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number

  int sum  = (lastEncoded2 << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue2 --;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue2 ++;

  lastEncoded2 = encoded; //store this value for next time

  //  if (encoded == 0b0000 || encoded == 0b0011) encoderValue2 ++;
  //  else if (encoded == 0b0001 || encoded == 0b0010) encoderValue2 --;
}

void updateEncoder3() {
  int MSB = digitalRead(EN31); //MSB = most significant bit
  int LSB = digitalRead(EN32); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded3 << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue3 --;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue3 ++;

  lastEncoded3 = encoded; //store this value for next time
}

void drive2(int n) {
  if (n > 255) {
    n = 255;
  } else if (n < -255) {
    n = -255;
  }

  if (n > 0) {
    analogWrite(IN21, n);
    analogWrite(IN22, 0);
  } else if (n < 0) {
    analogWrite(IN21, 0);
    analogWrite(IN22, -n);
  } else {
    analogWrite(IN21, 0);
    analogWrite(IN22, 0);
  }
}
void drive3(int n) {
  if (n > 0) {
    analogWrite(IN31, n);
    analogWrite(IN32, 0);
  } else if (n < 0) {
    analogWrite(IN31, 0);
    analogWrite(IN32, -n);
  } else {
    analogWrite(IN31, 0);
    analogWrite(IN32, 0);
  }
}

void updateRpm() {
  movement2 = encoderValue2 - oldEncoder2;
  movement3 = encoderValue3 - oldEncoder3;

  if (movement2 != 0) {
    int time = (millis() - timeold2);
    timeold2 = millis();
    oldEncoder2 = encoderValue2;
    rpm2 = 100 * movement2 / time;
  }

  if (movement3 != 0) {
    int time = (millis() - timeold3);
    timeold3 = millis();
    oldEncoder3 = encoderValue3;
    rpm3 = 100 * movement3 / time;
  }

  if (target2 == 0) {
    speed2 = 0;
  } else if (rpm2 < target2 && speed2 < 255) {
    speed2 = speed2 + change;
  } else if (rpm2 > target2 && speed2 > -255) {
    speed2 = speed2 - change;
  }
  if (target3 == 0) {
    speed3 = 0;
  } else if (rpm3 < target3 && speed3 < 255) {
    speed3 = speed3 + change;
  } else if (rpm3 > target3 && speed3 > -255) {
    speed3 = speed3 - change;
  }
}

void normalize() {
  int max = target1;
  if (target2 > max) {
    max = target2;
  }
  if (target3 > max) {
    max = target3;
  }

  if (max > 100) {
    double norm = 100.0 / max;
    target1 = target1 * norm;
    target2 = target2 * norm;
    target3 = target3 * norm;
  }
}



dclass::dclass() {
}

void dclass::SETUP() {
  pinMode(IN21, OUTPUT);
  pinMode(IN22, OUTPUT);
  pinMode(IN31, OUTPUT);
  pinMode(IN32, OUTPUT);
  pinMode(EN21, INPUT);
  pinMode(EN22, INPUT);
  pinMode(EN31, INPUT);
  pinMode(EN32, INPUT);

  target1 = 0;
  rpm2 = 0;
  target2 = 0;
  rpm3 = 0;
  target3 = 0;

  digitalWrite(EN21, HIGH); //turn pullup resistor on
  digitalWrite(EN22, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(digitalPinToInterrupt(EN21), updateEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EN22), updateEncoder2, CHANGE);

  digitalWrite(EN31, HIGH); //turn pullup resistor on
  digitalWrite(EN32, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(digitalPinToInterrupt(EN31), updateEncoder3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EN32), updateEncoder3, CHANGE);
}

void dclass::CALCULATE(int x, int y, int r) {
  int val = (r - x) / 3;
  target1 = x + val;
  target2 = val - y * 0.5774;
  target3 = val + y * 0.5774;
  normalize();
}

void dclass::DRIVE() {
  updateRpm();

  drive2(speed2);
  drive3(speed3);

}

dclass drive = dclass();


