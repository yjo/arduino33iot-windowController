#include "LedFxStrip.h"

void LedFxStrip::init(int fxMode, int brightness, int period_ms) {
  uint32_t colors[] = {RED, GREEN, BLUE};
  ws2812fx.init();
  ws2812fx.setColors(0, colors);
  setBrightness(brightness);
  setPeriod_ms(period_ms);
  setMode(fxMode);
  ws2812fx.start();
}

void LedFxStrip::setMode(int mode) {
  mode = constrain(mode, 0, FX_MODE_CUSTOM - 1);
  ws2812fx.setMode(mode);
  Serial.print(mode);
  Serial.print(": ");
  Serial.println(ws2812fx.getModeName(mode));
}

void LedFxStrip::setPeriod_ms(uint32_t period_ms) {
  // Keep speeds to form (10 * 2^n) for simplicity
  period_ms = constrain(period_ms, 10, 40960);
  ws2812fx.setSpeed(period_ms);
  Serial.print("Spd: ");
  Serial.println(period_ms);
}

void LedFxStrip::setBrightness(uint16_t newBrightness) {
  newBrightness = constrain(newBrightness, 0, 255);
  ws2812fx.setBrightness(newBrightness);
  Serial.print("Brightness: ");
  Serial.println(newBrightness);
}
