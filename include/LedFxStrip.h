#ifndef IOT33PIO_LEDFXSTRIP_H
#define IOT33PIO_LEDFXSTRIP_H

#include <WS2812FX.h>
#include "Config.h"

class LedFxStrip: private ConfigSubscriber {
  public:
    LedFxStrip(int ledCount, int ledPin) : ws2812fx(ledCount, ledPin, NEO_GRB | NEO_KHZ800) {}

    void init();

    void service();

    uint8_t getMode() { return ws2812fx.getMode(); }
    void setMode(int mode, Print &out = Serial);

    uint16_t getPeriod_ms() { return ws2812fx.getSpeed(); }
    void setPeriod_ms(uint32_t delay_ms, Print &out = Serial);

    uint8_t getBrightness() { return ws2812fx.getBrightness(); }
    void setBrightness(uint16_t newBrightness, Print &out = Serial);

  private:
    WS2812FX ws2812fx;
    uint8_t savedBrightness;

    void onConfigChanged() override;
    void onBeforeUpdate() override;
    void onUpdateFailed() override;
};

#endif //IOT33PIO_LEDFXSTRIP_H
