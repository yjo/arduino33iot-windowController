#include <Arduino.h>
#include <WiFiNINA.h>

#include "wifi_secrets.h"
#include "SlatsMotor.h"
#include "LedFxStrip.h"

const char wifiSsid[] = WIFI_SSID;
const char wifiPassword[] = WIFI_PASS;

LedFxStrip ledFxStrip = LedFxStrip(120, 13);
SlatsMotor slatsMotor;

void connectToWifi();
void updateParamsFromSerialInputChar();
size_t printWifiStatusTo(Print &p);

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");

  connectToWifi();
  ledFxStrip.init();
  slatsMotor.init(A7);
}

void loop() {
  updateParamsFromSerialInputChar();
  ledFxStrip.service();
  slatsMotor.service();
}

inline void connectToWifi() {
  for (int status = WL_IDLE_STATUS; status != WL_CONNECTED;) {
    Serial.println("Connecting to wifi...");
    status = WiFi.begin(wifiSsid, wifiPassword);
  }

  printWifiStatusTo(Serial);
}

size_t printWifiStatusTo(Print &p) {
  return
      p.print("WiFi firmware: v") + p.println(WiFiClass::firmwareVersion()) +
      p.print("IP: ") + p.println(WiFi.localIP()) +
      p.print("time: ") + p.println(WiFi.getTime());
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
      ledFxStrip.setBrightness((ledFxStrip.getBrightness() << 1) + 1);
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
