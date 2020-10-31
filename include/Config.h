#ifndef IOT33PIO_CONFIG_H
#define IOT33PIO_CONFIG_H

#include <WS2812FX.h>
#include "util.h"

#if defined(ARDUINO_SAMD_NANO_33_IOT)
#include <HttpClient.h>
#include <WiFiNINA.h>
#endif

class ConfigSubscriber {
  public:
    virtual void onBeforeUpdate() {};
    virtual void onConfigChanged() = 0;
    virtual void onUpdateFailed() {};
};

class ConfigClass {
  public:
    void subscribe(ConfigSubscriber *subscriber);

    // Cheeky global:
    bool isInBoo;

    // Config variables:
    uint8_t brightness = 0;
    uint32_t period_ms = 200;
    int fxMode = 0;
    uint32_t colours[3] = {RED, ORANGE, BLACK};
    uint8_t motorMode = 3; //stop

    uint32_t booColour = WHITE;
    uint8_t booBrightness = 255;
    int booFxMode = FX_MODE_STROBE;

#if defined(ARDUINO_SAMD_NANO_33_IOT)
    static constexpr char configServer[] = "home.yjo.me";
    static constexpr char configPath[] = "/ard/config.txt";
    void updateFromWeb();
#endif

  private:
    void set(const char *name, const char *value);
    void dispatchBeforeUpdate();
    void dispatchConfigChanged();
    void dispatchUpdateFailed();


    struct Subscriptions {
      ConfigSubscriber *const subscriber;
      Subscriptions *const next;
    };
    Subscriptions *subscriptions = nullptr;
    boolean updateFromStream(Stream &stream);

#if defined(ARDUINO_SAMD_NANO_33_IOT)
    bool updateSelfFromHttpClient(HttpClient &client);
    WiFiSSLClient wifiClient = WiFiSSLClient();
#endif
};

extern ConfigClass config;

#endif //IOT33PIO_CONFIG_H
