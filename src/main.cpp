#include <Arduino.h>
#include <WiFiNINA.h>

#include "Config.h"
#include "wifi_secrets.h"
#include "SlatsMotor.h"
#include "LedFxStrip.h"

const char wifiSsid[] = WIFI_SSID;
const char wifiPassword[] = WIFI_PASS;

LedFxStrip ledFxStrip = LedFxStrip(120, 13);
SlatsMotor slatsMotor;
WiFiServer telnetServer(23);

void connectToWifi();
void handleCommand(Stream &stream);
size_t printWifiStatusTo(Print &p);
void serviceTelnet();

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");

  connectToWifi();
  config.updateFromWeb();
  telnetServer.begin();
  ledFxStrip.init();
  slatsMotor.init(A7);
}

void loop() {
  handleCommand(Serial);
  serviceTelnet();
  ledFxStrip.service();
  slatsMotor.service();
}

inline void connectToWifi() {
  for (int status = WL_IDLE_STATUS; status != WL_CONNECTED;) {
    Serial.println("Connecting to wifi...");
    status = WiFi.begin(wifiSsid, wifiPassword);
    printWifiStatusTo(Serial);
  }
}

size_t printWifiStatusTo(Print &p) {
  return
      // see WiFiNINA/src/utility/wl_definitions.h:50 for list of statuses
      p.print("\nWiFi status: ") + p.println(WiFi.status()) +
      p.print("RSSI: ") + p.print(WiFi.RSSI()) + p.println("dBm") +
      p.print("Firmware: v") + p.println(WiFiClass::firmwareVersion()) +
      p.print("IP: ") + p.println(WiFi.localIP()) +
      p.print("time: ") + p.println(WiFi.getTime());
}

void handleCommand(Stream &stream) {
  switch (stream.read()) {
    case '8':
      ledFxStrip.setMode(ledFxStrip.getMode() + 1, stream);
      break;
    case '2':
      ledFxStrip.setMode(ledFxStrip.getMode() - 1, stream);
      break;
    case '7':
      ledFxStrip.setPeriod_ms(ledFxStrip.getPeriod_ms() << 1, stream);
      break;
    case '1':
      ledFxStrip.setPeriod_ms(ledFxStrip.getPeriod_ms() >> 1, stream);
      break;
    case '9':
      ledFxStrip.setBrightness((ledFxStrip.getBrightness() << 1) + 1, stream);
      break;
    case '3':
      ledFxStrip.setBrightness(min(255, ledFxStrip.getBrightness() >> 1), stream);
      break;
    case '4':
      slatsMotor.setMode(SlatsMotor::Mode::closed, stream);
      break;
    case '5':
      slatsMotor.setMode(SlatsMotor::Mode::boo, stream);
      break;
    case '6':
      slatsMotor.setMode(SlatsMotor::Mode::open, stream);
      break;
    case '0':
      slatsMotor.setMode(SlatsMotor::Mode::stop, stream);
      ledFxStrip.setBrightness(0);
    case '?':
      printWifiStatusTo(stream);
      break;
    case '!': // echo to all terminals
      Serial.println('!');
      telnetServer.println('!');
      break;
    case 'u':
      config.updateFromWeb();
      break;
    case 'r':
      NVIC_SystemReset();
      break;
    default:
      break;
  }
}

void serviceTelnet() {
  while (auto client = telnetServer.available()) {
    handleCommand(client);
  }
}
