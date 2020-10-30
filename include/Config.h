#ifndef IOT33PIO_CONFIG_H
#define IOT33PIO_CONFIG_H

#include <WS2812FX.h>
#include <HttpClient.h>
#include <WiFiNINA.h>
#include "util.h"

class ConfigSubscriber {
  public:
    virtual void onBeforeUpdate() {};
    virtual void onConfigChanged() = 0;
    virtual void onUpdateFailed() {};
};

class ConfigClass {
  public:
    static constexpr char configServer[] = "home.yjo.me";
    static constexpr char configPath[] = "/ard/config.txt";
    void updateFromWeb();

    uint8_t brightness = 0;
    uint32_t period_ms = 200;
    int fxMode = 0;
    uint32_t colours[3] = {RED, ORANGE, BLACK};
    uint8_t motorMode = 3; //stop

    void subscribe(ConfigSubscriber *subscriber);
  private:
    void set(const char *name, const char *value);
    void dispatchBeforeUpdate();
    void dispatchConfigChanged();
    void dispatchUpdateFailed();

    bool updateSelfFromHttpClient(HttpClient &client);

    struct Subscriptions {
      ConfigSubscriber *const subscriber;
      Subscriptions *const next;
    };
    Subscriptions *subscriptions = nullptr;
    WiFiSSLClient wifiClient = WiFiSSLClient();
    boolean updateFromStream(Stream &stream);
};

extern ConfigClass config;

#endif //IOT33PIO_CONFIG_H
