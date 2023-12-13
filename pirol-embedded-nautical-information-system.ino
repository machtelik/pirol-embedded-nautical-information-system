#include <Arduino.h>

#include "Config.h"
#include "Utils.h"

#include "Display.h"
#include "Sensors.h"

TCA tca(TCA_ADDRESS, TCA_RESET_PIN);

Display displays[] = {
    Display(DISPLAY_ADDRESS_0, tca), Display(DISPLAY_ADDRESS_1, tca),
    Display(DISPLAY_ADDRESS_2, tca), Display(DISPLAY_ADDRESS_3, tca),
    Display(DISPLAY_ADDRESS_4, tca)};

VoltageSensor input_voltage_sensor(INPUT_VOLATGE_PIN);
VoltageSensor external_5V_voltage_sensor(EXTERNAL_5V_POWER_PIN);
TempHumSensor temp_hum_sensor(TEMP_HUM_SENSOR_PIN);
UptimeSensor uptime_sensor;
TempSensors temp_sensors(TEMP_PIN);

bool did_external_setup = false;

void setupInternalDevices() {
  input_voltage_sensor.setup();
  external_5V_voltage_sensor.setup();
  uptime_sensor.setup();
}

void setupExternalDevices() {
  tca.enable();
  temp_hum_sensor.setup();
  temp_sensors.setup();
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    displays[i].setup();
  }
  did_external_setup = true;
}

void reset() {
  if (!did_external_setup) {
    return;
  }
  did_external_setup = false;

  tca.disable();

  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    displays[i].reset();
  }

  delay(5);
}

bool checkI2CState() {
  if (!Wire.getWireTimeoutFlag()) {
    return true;
  }

  Wire.clearWireTimeoutFlag();

  return false;
}

void setup() {
  Serial.begin(9600);

  // Wait a bit for everything to come up
  delay(50);

  Wire.setWireTimeout(2000, true);
  // Switch to I2C fast mode
  Wire.setClock(400000);

  setupInternalDevices();

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
  // Make sure we have external power first
  external_5V_voltage_sensor.update(true);
  if (external_5V_voltage_sensor.getValue() < EXTERNAL_SENSOR_VOLTAGE_HIGH) {
    reset();
    return;
  }

  if (!checkI2CState()) {
    reset();
  }

  if (!did_external_setup) {
    setupExternalDevices();
  }

  if (input_voltage_sensor.update()) {
    displays[2].setText(
        3, "Eingang: " + String(input_voltage_sensor.getValue()) + "mV");
  }

  if (temp_hum_sensor.update()) {
    displays[1].setText(0, "Luft: " + String(temp_hum_sensor.getTemp()) + "C");
    displays[1].setText(
        1, "Feuchte: " + String(temp_hum_sensor.getHumidity()) + "%");
  }

  if (uptime_sensor.update()) {
    displays[4].setText(3,
                        "Uptime: " + String(uptime_sensor.getUptime()) + "s");
  }

  // Update displays
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    displays[i].update();
  }
}
