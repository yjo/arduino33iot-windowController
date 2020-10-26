#include <SNU.h>
_SFU_H_INCLUDED // suppress unused header inspection
#include <WiFiNINA.h>

#include "arduino_secrets.h"

const char wifiSsid[] = WIFI_SSID;
const char wifiPassword[] = WIFI_PASS;
const char url[] = SECRET_OTA_URL;

int status = WL_IDLE_STATUS;

String message;

void setup() {
  Serial.begin(256000);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifiSsid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(wifiSsid, wifiPassword);
  }

  // wait a bit
  delay(1000);

  Serial.print("WiFi fw version: ");
  Serial.println(WiFi.firmwareVersion());
  Serial.print("MAC: ");
  auto macAddr = WiFiDrv::getMacAddress();
  for (int i = 0; i < WL_MAC_ADDR_LENGTH; ++i) {
    Serial.print(macAddr[i], HEX);
  }
  Serial.println();

  message += "Sketch compile date and time: ";
  message += __DATE__;
  message += " ";
  message += __TIME__;

  // print out the sketch compile date and time on the serial port
  Serial.println(message);
//  Serial1.println(message);

  Serial.println("Type \"download\" in the Serial Monitor to start downloading the update");
}

void loop() {
  String command = Serial.readStringUntil('\n');

  if (command.indexOf("download") == 0) {

    Serial.println("Downloading update file");
    //    WiFiStorage.download(url, "UPDATE.BIN");

    const char *filename = "UPDATE.BIN";
    int8_t dlRetVal = WiFiDrv::downloadFile(url, strlen(url), filename, strlen(filename));
    Serial.println("dlretval: " + String(dlRetVal));

    WiFiStorageFile update = WiFiStorage.open("/fs/UPDATE.BIN");
    if (update.available()) {
      Serial.println("Download complete, please restart or type \"restart\" to apply the update");
      Serial.println("Filesize: " + String(update.available()));
    } else {
      Serial.println("Download failed, please retry :(");
    }
  } else if (command.indexOf("restart") >= 0) {
    NVIC_SystemReset();
  }
}
