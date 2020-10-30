#include "Config.h"
#include "SlatsMotor.h"

void ConfigClass::updateFromWeb() {
  uint32_t start_ms = millis();
  Serial.println("Updating...");
  wifiClient.stop();
  HttpClient client(wifiClient);

  dispatchBeforeUpdate();
  if (updateSelfFromHttpClient(client)) {
    dispatchConfigChanged();
    Serial.println("Update successful");
  } else {
    dispatchUpdateFailed();
  }
  client.stop();
  Serial.print(millis() - start_ms);
  Serial.println("ms elapsed");
}

bool ConfigClass::updateSelfFromHttpClient(HttpClient &client) {
  int result;

  if ((result = client.get(configServer, 443, configPath)) != 0) {
    Serial.print("ERROR: client.get returned ");
    Serial.println(result);
    return false;
  }

  if ((result = client.responseStatusCode()) != 200) {
    Serial.print("ERROR: unexpected HTTP response code ");
    Serial.println(result);
    return false;
  }

  if ((result = client.skipResponseHeaders()) < 0) {
    Serial.print("ERROR: client.skipResponseHeaders() returned ");
    Serial.println(result);
    return false;
  }

  return updateFromStream(client);
}

bool ConfigClass::updateFromStream(Stream &stream) {
  while (stream.available()) {
    // skip whitespace
    int nextChar = 0;
    do {
      nextChar && stream.read();
      nextChar = stream.peek();
    } while(nextChar == ' ' || nextChar == '\r' || nextChar == '\n' || nextChar == '\t');

    if (nextChar == '#') {
      stream.find('\n');
      continue;
    }

    String name = stream.readStringUntil('=');
    String value = stream.readStringUntil(';');
    if (name.length() && value.length()) {
      set(name.c_str(), value.c_str());
    }
  }
}

void ConfigClass::subscribe(ConfigSubscriber *subscriber) {
  subscriptions = new Subscriptions{subscriber, subscriptions};
  subscriber->onConfigChanged();
}

void ConfigClass::dispatchBeforeUpdate() {
  for (const Subscriptions *ptr = subscriptions; ptr != nullptr; ptr = ptr->next) {
    ptr->subscriber->onBeforeUpdate();
  }
}

void ConfigClass::dispatchConfigChanged() {
  for (const Subscriptions *ptr = subscriptions; ptr != nullptr; ptr = ptr->next) {
    ptr->subscriber->onConfigChanged();
  }
}

void ConfigClass::dispatchUpdateFailed() {
  for (const Subscriptions *ptr = subscriptions; ptr != nullptr; ptr = ptr->next) {
    ptr->subscriber->onUpdateFailed();
  }
}

void ConfigClass::set(const char *name, const char *value) {
  Serial.print("Setting '") + Serial.print(name) + Serial.print("' = '") + Serial.print(value) + Serial.println("'");
  if (!strcmp(name, "brightness")) {
    brightness = strtoul(value, nullptr, 10);
  } else if (!strcmp(name, "period_ms")) {
    period_ms = strtoul(value, nullptr, 10);
  } else if (!strcmp(name, "fxMode")) {
    fxMode = strtoul(value, nullptr, 10);
  } else if (!strcmp(name, "colour0")) {
    colours[0] = strtoul(value, nullptr, 16);
  } else if (!strcmp(name, "colour1")) {
    colours[1] = strtoul(value, nullptr, 16);
  } else if (!strcmp(name, "colour2")) {
    colours[2] = strtoul(value, nullptr, 16);
  } else if (!strcmp(name, "motorMode")) {
    switch (*value){
      case 'b':
        motorMode = static_cast<uint8_t>(SlatsMotor::Mode::boo);
        break;
      case 'c':
        motorMode = static_cast<uint8_t>(SlatsMotor::Mode::closed);
        break;
      case 'o':
        motorMode = static_cast<uint8_t>(SlatsMotor::Mode::open);
        break;
      case 's':
        motorMode = static_cast<uint8_t>(SlatsMotor::Mode::stop);
        break;
    };
  }else {
    Serial.print("Unknown property: '");
    Serial.print(name);
    Serial.println("'");
  }
}

constexpr char ConfigClass::configServer[];
constexpr char ConfigClass::configPath[];

ConfigClass config;
