#include <Arduino.h>

#include "Display.h"
#include "Sensors.h"
#include "Utils.h"

// PINs
#define INPUT_VOLATGE_PIN A0
#define TEMP_HUM_SENSOR_PIN 3
#define TEMP_PIN 5

// i2c bus addresses
#define DISPLAY_ADDRESS_0 0x22
#define DISPLAY_ADDRESS_1 0x23
#define DISPLAY_ADDRESS_2 0x25
#define DISPLAY_ADDRESS_3 0x26
#define DISPLAY_ADDRESS_4 0x27

Display displays[] = {Display(DISPLAY_ADDRESS_0), Display(DISPLAY_ADDRESS_1),
                      Display(DISPLAY_ADDRESS_2), Display(DISPLAY_ADDRESS_3),
                      Display(DISPLAY_ADDRESS_4)};

VoltageSensor input_voltage_sensor(INPUT_VOLATGE_PIN);
TempHumSensor temp_hum_sensor(TEMP_HUM_SENSOR_PIN);
UptimeSensor uptime_sensor;
TempSensors temp_sensors(TEMP_PIN);

void setup() {
  Serial.begin(9600);

  // Wait a bit for everything to come up
  delay(500);

  Wire.setWireTimeout(2000, true);
  // Switch to I2C fast mode
  Wire.setClock(400000);

  input_voltage_sensor.setup();
  temp_hum_sensor.setup();
  uptime_sensor.setup();
  temp_sensors.setup();
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    displays[i].setup();
  }

  displays[0].setText(0, "Kuehlwasser:     / C");
  displays[0].setText(1, "Motor Gehaeuse:  / C");
  displays[0].setText(2, "Motoroel:        / C");
  displays[0].setText(3, "Umgebung:        / C");

  displays[1].setText(2, "Wasser:          / C");
  displays[1].setText(3, "Druck:         / hPa");

  displays[2].setText(0, "Battery A:       / V");
  displays[2].setText(1, "Battery B:       / V");
  displays[2].setText(2, "Verbrauch:       / W");

  displays[3].setText(0, "Diesel:          / %");
  displays[3].setText(1, "Wasser:          / %");
  displays[3].setText(2, "Abwasser:        / %");
  displays[3].setText(3, "Bier:          3/120");

  displays[4].setText(0, "Was ist rot und");
  displays[4].setText(1, "wiehert?");
  displays[4].setText(2, "Eine Pferdbeere!");
}

void loop() {
  if (input_voltage_sensor.update()) {
    displays[2].setText(3, "Eingang: " + String(input_voltage_sensor.getValue()) + "mV");
  }

  if (temp_hum_sensor.update()) {
    displays[1].setText(0, "Luft: " + String(temp_hum_sensor.getTemp()) + "C");
    displays[1].setText(1, "Feuchte: " + String(temp_hum_sensor.getHumidity()) + "%");
  }

  if (uptime_sensor.update()) {
    displays[4].setText(3, "Uptime: " + String(uptime_sensor.getUptime()) + "s");
  }

  // Update displays
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    displays[i].update();
  }

  if (Wire.getWireTimeoutFlag()) {
    for (int i = 0; i < DISPLAY_COUNT; ++i) {
      displays[i].reset();
    }
    Wire.clearWireTimeoutFlag();
  }
}
