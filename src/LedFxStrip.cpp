#include "LedFxStrip.h"

void LedFxStrip::init() {
  ws2812fx.init();
  config.subscribe(this);
  ws2812fx.start();
}

void LedFxStrip::onConfigChanged() {
  ws2812fx.setColors(0, config.colours);
  setBrightness(config.brightness);
  setPeriod_ms(config.period_ms);
  setMode(config.fxMode);
}

void LedFxStrip::setMode(int mode, Print &out) {
  mode = constrain(mode, 0, FX_MODE_CUSTOM - 1);
  ws2812fx.setMode(mode);
  out.print(mode);
  out.print(": ");
  out.println(ws2812fx.getModeName(mode));
}

void LedFxStrip::setPeriod_ms(uint32_t period_ms, Print &out) {
  // Keep speeds to form (10 * 2^n) for simplicity
  period_ms = constrain(period_ms, 10, 40960);
  ws2812fx.setSpeed(period_ms);
  out.print("Spd: ");
  out.println(period_ms);
}

void LedFxStrip::setBrightness(uint16_t newBrightness, Print &out) {
  newBrightness = constrain(newBrightness, 0, 255);
  ws2812fx.setBrightness(newBrightness);
  out.print("Brightness: ");
  out.println(newBrightness);
}
