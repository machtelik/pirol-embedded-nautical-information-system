#pragma once

#include <OneWire.h>
#include <DHT.h>

#define TEMP_HUM_SENSOR_PIN 3
#define TEMP_PIN 5

#define ANALOG_VALUE_TO_VOLTAGE 30 // 1 digit = 0,03V (30,0mV)

#define INPUT_VOLATGE_PIN A0

class VoltageSensor
{
public:
  VoltageSensor(int pin) : pin(pin), value(0){};
  bool update()
  {
    int new_value = analogRead(pin);
    new_value = new_value * ANALOG_VALUE_TO_VOLTAGE;
    if (new_value != value)
    {
      value = new_value;
      return true;
    }
    return false;
  }

  int getValue()
  {
    return value;
  }

private:
  int pin;
  float value;
};

class TempHumSensor
{
public:
  TempHumSensor(int pin) : dht(pin, DHT22), temp(0.0), hum(0.0)
  {
    dht.begin();
  }
  bool update()
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (t != temp || h != hum)
    {
      temp = t;
      hum = h;
      return true;
    }

    return false;
  }

  float getTemp() { return temp; }
  float getHumidity() { return hum; }

private:
  DHT dht;
  float temp;
  float hum;
};

VoltageSensor input_voltage_sensor(INPUT_VOLATGE_PIN);
TempHumSensor temp_hum_sensor(TEMP_HUM_SENSOR_PIN);
