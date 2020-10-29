#ifndef IOT33PIO_LEDFXSTRIP_H
#define IOT33PIO_LEDFXSTRIP_H

#include <WS2812FX.h>

class LedFxStrip {

  public:
    LedFxStrip(int ledCount, int ledPin) : ws2812fx(ledCount, ledPin, NEO_GRB | NEO_KHZ800) {}

    void init(int fxMode = FX_MODE_RAINBOW_CYCLE, int brightness = 31, int period_ms = 320);

    void service() {
      ws2812fx.service();
    }

    uint8_t getMode() { return ws2812fx.getMode(); }
    void setMode(int mode, Print &out = Serial);

    uint16_t getPeriod_ms() { return ws2812fx.getSpeed(); }
    void setPeriod_ms(uint32_t delay_ms, Print &out = Serial);

    uint8_t getBrightness() { return ws2812fx.getBrightness(); }
    void setBrightness(uint16_t newBrightness, Print &out = Serial);

  private:
    WS2812FX ws2812fx;
};


#endif //IOT33PIO_LEDFXSTRIP_H
