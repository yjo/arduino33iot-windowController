#include <Arduino.h>
#include "SlatsMotor.h"
#include "util.h"

SlatsMotor slatsMotor;

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");

  slatsMotor.init(A7);

  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  slatsMotor.service();
}
