#pragma once

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ANALOG_VALUE_TO_VOLTAGE 30  // 1 digit = 0,03V (30,0mV)

#define VOLTAGE_UPDATE_PERIODE 5000
#define TEMP_HUM_UPDATE_PERIODE 5000
#define UPTIME_UPDATE_PERIODE 5000
#define TEMP_UPDATE_PERIODE 5000


class VoltageSensor {
 public:
  VoltageSensor(int pin) : is_setup(false), pin(pin), value(0), lastUpdate(0){};

  void setup() {
    if (is_setup) {
      return;
    }

    pinMode(pin, INPUT);
    is_setup = true;
  }

  bool update() {
    if (!is_setup) {
      return false;
    }

    if (millis() - lastUpdate < VOLTAGE_UPDATE_PERIODE) {
      return;
    }

    int new_value = analogRead(pin) * ANALOG_VALUE_TO_VOLTAGE;
    lastUpdate = millis();
    if (new_value != value) {
      value = new_value;
      return true;
    }
    return false;
  }

  int getValue() { return value; }

 private:
  bool is_setup;
  int pin;
  float value;
  unsigned long lastUpdate;
};

class TempHumSensor {
 public:
  TempHumSensor(int pin)
      : is_setup(false), dht(pin, DHT22), temp(0.0), hum(0.0), lastUpdate(0) {}

  void setup() {
    if (is_setup) {
      return;
    }

    dht.begin();
    is_setup = true;
  }

  bool update() {
    if (!is_setup) {
      return false;
    }

    if (millis() - lastUpdate < TEMP_HUM_UPDATE_PERIODE) {
      return;
    }

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    lastUpdate = millis();

    if (t != temp || h != hum) {
      temp = t;
      hum = h;
      return true;
    }

    return false;
  }

  float getTemp() { return temp; }
  float getHumidity() { return hum; }

 private:
  bool is_setup;
  DHT dht;
  float temp;
  float hum;
  unsigned long lastUpdate;
};

class UptimeSensor {
 public:
  UptimeSensor() : lastUpdate(0) {}

  void setup() { lastUpdate = millis(); }

  bool update() {
    if (millis() - lastUpdate < UPTIME_UPDATE_PERIODE) {
      return false;
    }

    lastUpdate = millis();
    return true;
  }

  int getUptime() { return lastUpdate / 1000; }

 private:
  unsigned long lastUpdate;
};

class TempSensors {
 public:
  TempSensors(int pin) : lastUpdate(0), oneWire(pin), sensors(&oneWire) {
    }

  void setup() { 
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  }

  bool update() {
    if (millis() - lastUpdate < UPTIME_UPDATE_PERIODE) {
      return false;
    }

    lastUpdate = millis();
    return true;
  }


 private:
  unsigned long lastUpdate;
  OneWire oneWire;
  DallasTemperature sensors;
};
