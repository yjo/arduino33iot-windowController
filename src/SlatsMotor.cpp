#include "Arduino.h"
#include "SlatsMotor.h"

constexpr SlatsMotor::ServoInstr SlatsMotor::servoProg[];

void SlatsMotor::init(int pinNumber) {
  servo.attach(pinNumber);
  currentInstrStart_ms = currentInstrEnd_ms = millis();
}

void SlatsMotor::service() {
  switch (mode) {
    case Mode::closed:
      angle_deg = SLATS_CLOSED_deg;
      break;
    case Mode::open:
      angle_deg = SLATS_OPEN_deg;
      break;
    case Mode::boo:
      serviceProgram();
      break;
    case Mode::stop:
      break;
  }

  angle_deg = constrain(angle_deg, SLATS_OPEN_deg, SLATS_CLOSED_deg);
  int delay_us = map(angle_deg, 0, 270, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  servo.writeMicroseconds(delay_us);
}

void SlatsMotor::serviceProgram() {
  const uint32_t now_ms = millis();

  const uint32_t duration_ms = currentInstrEnd_ms - currentInstrStart_ms;
  const uint32_t elapsed_ms = constrain(now_ms - currentInstrStart_ms, 0, duration_ms);
  angle_deg = map(elapsed_ms, 0, duration_ms, currentInstrStartAngle_deg, currentInstr->endAngle_deg);

  if (isXAfterY_wrapped(now_ms, currentInstrEnd_ms)) {
    programTick(now_ms);
  }
}

void SlatsMotor::programTick(const uint32_t now_ms) {
  angle_deg = currentInstr->endAngle_deg;

  auto nextInstrIndex = (currentInstr - servoProg + 1) % servoProgSize;
  currentInstr = &servoProg[nextInstrIndex];

  currentInstrStart_ms = now_ms;
  currentInstrStartAngle_deg = angle_deg;
  uint32_t delay_ms = random(currentInstr->minDuration_ms,
                             max(currentInstr->minDuration_ms, currentInstr->maxDuration_ms));
  currentInstrEnd_ms = now_ms + delay_ms;
}

void SlatsMotor::setMode(SlatsMotor::Mode mode) {
  switch (mode) {
    case Mode::closed:
      Serial.println("slats: closed");
      break;
    case Mode::open:
      Serial.println("slats: open");
      break;
    case Mode::boo:
      Serial.println("slats: boo!");
      currentInstrStartAngle_deg = angle_deg;
      break;
  }
  this->mode = mode;
}
