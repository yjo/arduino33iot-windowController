#include <Arduino.h>

#include "SlatsMotor.h"
#include "LedFxStrip.h"

LedFxStrip ledFxStrip = LedFxStrip(120, 13);
SlatsMotor slatsMotor;

void updateParamsFromSerialInputChar();

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");

  ledFxStrip.init();
  slatsMotor.init(A7);
}

void loop() {
  updateParamsFromSerialInputChar();
  ledFxStrip.service();
  slatsMotor.service();
}

void updateParamsFromSerialInputChar() {
  switch (Serial.read()) {
    case '8':
      ledFxStrip.setMode(ledFxStrip.getMode() + 1);
      break;
    case '2':
      ledFxStrip.setMode(ledFxStrip.getMode() - 1);
      break;
    case '7':
      ledFxStrip.setPeriod_ms(ledFxStrip.getPeriod_ms() << 1);
      break;
    case '1':
      ledFxStrip.setPeriod_ms(ledFxStrip.getPeriod_ms() >> 1);
      break;
    case '9':
      ledFxStrip.setBrightness( (ledFxStrip.getBrightness() << 1) + 1);
      break;
    case '3':
      ledFxStrip.setBrightness(min(255, ledFxStrip.getBrightness() >> 1));
      break;
    case '4':
      slatsMotor.setMode(SlatsMotor::Mode::closed);
      break;
    case '5':
      slatsMotor.setMode(SlatsMotor::Mode::boo);
      break;
    case '6':
      slatsMotor.setMode(SlatsMotor::Mode::open);
      break;
    case '0':
      slatsMotor.setMode(SlatsMotor::Mode::stop);
      ledFxStrip.setBrightness(0);
    default:
      break;
  }
}
