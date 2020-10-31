#include <Arduino.h>

#include "Config.h"
#include "SlatsMotor.h"
#include "LedFxStrip.h"

#if defined(ARDUINO_SAMD_NANO_33_IOT)
#include <WiFiNINA.h>
#include "wifi_secrets.h"

const char wifiSsid[] = WIFI_SSID;
const char wifiPassword[] = WIFI_PASS;
WiFiServer telnetServer(23);

void connectToWifi();
void serviceTelnet();
#endif

LedFxStrip ledFxStrip = LedFxStrip(120, RGB_LEDS_PIN);
SlatsMotor slatsMotor;

void handleCommand(Stream &stream);
size_t printStatusTo(Print &p);

void setup() {
  Serial.begin(256000);
  Serial.println("\n>>");

#if defined(ARDUINO_SAMD_NANO_33_IOT)
  connectToWifi();
  config.updateFromWeb();
  telnetServer.begin();
#endif
  ledFxStrip.init();
  slatsMotor.init(SLATS_MOTOR_PIN);
}

void loop() {
  handleCommand(Serial);
#if defined(ARDUINO_SAMD_NANO_33_IOT)
  serviceTelnet();
#endif
  ledFxStrip.service();
  slatsMotor.service();
}

#if defined(ARDUINO_SAMD_NANO_33_IOT)
inline void connectToWifi() {
  for (int status = WL_IDLE_STATUS; status != WL_CONNECTED;) {
    Serial.println("Connecting to wifi...");
    status = WiFi.begin(wifiSsid, wifiPassword);
    printStatusTo(Serial);
  }
}

void serviceTelnet() {
  while (auto client = telnetServer.available()) {
    handleCommand(client);
  }
}
#endif

size_t printStatusTo(Print &p) {
  return
#if defined(ARDUINO_SAMD_NANO_33_IOT)
      // see WiFiNINA/src/utility/wl_definitions.h:50 for list of statuses
      p.print("\nWiFi status: ") + p.println(WiFi.status()) +
      p.print("RSSI: ") + p.print(WiFi.RSSI()) + p.println("dBm") +
      p.print("Firmware: v") + p.println(WiFiClass::firmwareVersion()) +
      p.print("IP: ") + p.println(WiFi.localIP()) +
      p.print("time: ") + p.println(WiFi.getTime()) +
#endif
      p.println("Compiled at: " __DATE__ " " __TIME__);
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
      printStatusTo(stream);
      break;
#if defined(ARDUINO_SAMD_NANO_33_IOT)
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
#endif
    default:
      break;
  }
}
