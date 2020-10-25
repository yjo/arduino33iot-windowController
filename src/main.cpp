#include <Arduino.h>
#include <WS2812FX.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 120
#define LED_PIN 13
#define MAX_BRIGHTNESS 255

WS2812FX ledFx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void changeLedFxMode(int newLedFxMode) {
  newLedFxMode = constrain(newLedFxMode, 0, FX_MODE_CUSTOM);
  ledFx.setMode(newLedFxMode);
  Serial.print(newLedFxMode);
  Serial.print(": ");
  Serial.println(ledFx.getModeName(newLedFxMode));
}

void changeLedFxSpeed(uint16_t newSpeed) {
  newSpeed = constrain(newSpeed, SPEED_MIN, SPEED_MAX);
  ledFx.setSpeed(newSpeed);
  Serial.print("Spd: ");
  Serial.println(newSpeed);
}

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");
  ledFx.init();
  ledFx.setColor(WHITE);
  ledFx.setBrightness(MAX_BRIGHTNESS);
  changeLedFxSpeed(256);
  changeLedFxMode(FX_MODE_RAINBOW_CYCLE);
  ledFx.start();
}

void loop() {
  const uint32_t now_us = micros();
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
  }
  ledFx.service();
}
