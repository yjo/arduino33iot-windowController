#include <Arduino.h>
#include <WS2812FX.h>
#include <Adafruit_NeoPixel.h>

#include "SlatsMotor.h"

#define LED_COUNT 120
#define LED_PIN 13
#define DEFAULT_BRIGHTNESS 31

SlatsMotor slatsMotor;
WS2812FX ledFx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB | NEO_KHZ800);

uint32_t colors[] = {RED, GREEN, BLUE};

void changeLedFxMode(int newLedFxMode) {
  newLedFxMode = constrain(newLedFxMode, 0, FX_MODE_CUSTOM);
  ledFx.setMode(newLedFxMode);
  Serial.print(newLedFxMode);
  Serial.print(": ");
  Serial.println(ledFx.getModeName(newLedFxMode));
}

void changeLedFxSpeed(int32_t newSpeed) {
  newSpeed = constrain(newSpeed, SPEED_MIN, SPEED_MAX);
  ledFx.setSpeed(newSpeed);
  Serial.print("Spd: ");
  Serial.println(newSpeed);
}

void changeLedFxBrightness(int16_t newBrightness) {
  newBrightness = constrain(newBrightness, 0, 255);
  ledFx.setBrightness(newBrightness);
  Serial.print("Brightness: ");
  Serial.println(newBrightness);
}

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");
  ledFx.init();
  ledFx.setColors(0, colors);
  changeLedFxBrightness(DEFAULT_BRIGHTNESS);
  changeLedFxSpeed(256);
  changeLedFxMode(FX_MODE_RAINBOW_CYCLE);
  ledFx.start();

  slatsMotor.init(A7);

  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  switch (char c = Serial.read()) {
    case '8':
      changeLedFxMode(ledFx.getMode() + 1);
      break;
    case '2':
      changeLedFxMode(ledFx.getMode() - 1);
      break;
    case '7':
      changeLedFxSpeed(ledFx.getSpeed() << 1);
      break;
    case '1':
      changeLedFxSpeed(ledFx.getSpeed() >> 1);
      break;
    case '9':
      changeLedFxBrightness( (ledFx.getBrightness() << 1) + 1);
      break;
    case '3':
      changeLedFxBrightness(min(255, ledFx.getBrightness() >> 1));
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
    default:
      break;
  }
  ledFx.service();
  slatsMotor.service();
}
