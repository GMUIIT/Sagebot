#include <Arduino.h>
#include "TwoWayMotor.h"
#ifndef TWO_WAY_MOTOR_CPP
#define TWO_WAY_MOTOR_CPP

void TwoWayMotor::init() {
  pinMode(dirPinA, OUTPUT);
  pinMode(dirPinB, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(dirPinA, LOW);
  digitalWrite(dirPinB, LOW);
  digitalWrite(enPin, LOW);
}

void TwoWayMotor::write(int amount){
  if (amount < -255 || amount > 255) {
    return;
  }
  if (amount < 0) {
    digitalWrite(dirPinA, HIGH);
    digitalWrite(dirPinB, LOW);
    amount *= -1;
  } else if (amount > 0) {
    digitalWrite(dirPinA, LOW);
    digitalWrite(dirPinB, HIGH);
  } else {
    digitalWrite(dirPinA, LOW);
    digitalWrite(dirPinB, LOW);
  }
  analogWrite(enPin, amount);
}

#endif
