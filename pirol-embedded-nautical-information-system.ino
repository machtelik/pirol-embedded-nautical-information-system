#include <Arduino.h>

#include "Utils.h"
#include "Display.h"
#include "Sensors.h"

#define LOOP_DURATION 1000
unsigned long last_loop_millis = 0;

void setup()
{
  Serial.begin(9600);

  delay(500);

  setupDisplays();

  // Switch to I2C fast mode
  Wire.setClock(400000);

  displays[0].setText(0, "Kuehlwasser:     / C");
  displays[0].setText(1, "Motor Gehaeuse:  / C");
  displays[0].setText(2, "Motoroel:        / C");
  displays[0].setText(3, "Umgebung:        / C");

  displays[1].setText(0, "Luft:");
  displays[1].setText(1, "Feuchte:");
  displays[1].setText(2, "Wasser:          / C");
  displays[1].setText(3, "Druck:         / hPa");

  displays[2].setText(0, "Battery A:       / V");
  displays[2].setText(1, "Battery B:       / V");
  displays[2].setText(2, "Verbrauch:       / W");
  displays[2].setText(3, "Eingang:");

  displays[3].setText(0, "Diesel:          / %");
  displays[3].setText(1, "Wasser:          / %");
  displays[3].setText(2, "Abwasser:        / %");
  displays[3].setText(3, "Bier:          3/120");

  displays[4].setText(0, "Was ist rot und");
  displays[4].setText(1, "wiehert?");
  displays[4].setText(2, "--------------------");
  displays[4].setText(3, "Eine Pferdbeere!");
}

void loop()
{
  unsigned long current_millis = millis();

  if (last_loop_millis - current_millis < LOOP_DURATION)
  {
    return;
  }

  if (input_voltage_sensor.update())
  {
    displays[2].setText(3, "Eingang:     " + String(input_voltage_sensor.getValue()) + "mV");
  }

  if (temp_hum_sensor.update())
  {
    displays[1].setText(0, "Luft: " + String(temp_hum_sensor.getTemp()) + "C");
    displays[1].setText(1, "Feuchte: " + String(temp_hum_sensor.getHumidity()) + "%");
  }

  refreshDisplays();
  last_loop_millis = millis();
}
