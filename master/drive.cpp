
#include "Arduino.h"
#include "drive.h"

const double change = 0.05;

const uint8_t IN11 = 10;
const uint8_t IN12 = 11;
const uint8_t EN11 = 2;
const uint8_t EN12 = 3;

int target1, target2, target3;

volatile long encoderValue1 = 0;
volatile long lastEncoded1 = 0;

long oldEncoder1 = 0;

int rpm1 = 0;
double speed1 = 0;
int movement1;
unsigned long timeold1 = 0;

void updateEncoder1() {
  int MSB = digitalRead(EN11); //MSB = most significant bit
  int LSB = digitalRead(EN12); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded1 << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue1 --;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue1 ++;

  lastEncoded1 = encoded; //store this value for next time
}

void drive1(int n) {
  if (n > 255) {
    n = 255;
  } else if (n < -255) {
    n = -255;
  }

  if (n > 0) {
    analogWrite(IN11, n);
    analogWrite(IN12, 0);
  } else if (n < 0) {
    analogWrite(IN11, 0);
    analogWrite(IN12, -n);
  } else {
    analogWrite(IN11, 0);
    analogWrite(IN12, 0);
  }
}


void updateRpm() {
  movement1 = encoderValue1 - oldEncoder1;

  if (movement1 != 0) {
    int time = (millis() - timeold1);
    timeold1 = millis();
    oldEncoder1 = encoderValue1;
    rpm1 = 100 * movement1 / time;
  }

  if (target1 == 0) {
    speed1 = 0;
  } else if (rpm1 < target1 && speed1 < 255) {
    speed1 = speed1 + change;
  } else if (rpm1 > target1 && speed1 > -255) {
    speed1 = speed1 - change;
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
  pinMode(IN11, OUTPUT);
  pinMode(IN12, OUTPUT);
  pinMode(EN11, INPUT);
  pinMode(EN12, INPUT);


  target1 = 0;
  rpm1 = 0;
  target2 = 0;
  target3 = 0;

  digitalWrite(EN11, HIGH); //turn pullup resistor on
  digitalWrite(EN12, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(digitalPinToInterrupt(EN11), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EN12), updateEncoder1, CHANGE);
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

  drive1(speed1);

}

dclass drive = dclass();


