#include "Display.h"



void setup()
{
  Serial.begin(9600);

  delay(500);

  setupDisplays();

  // Switch to I2C fast mode
  Wire.setClock(400000);

  displays[0].setText(0, "Kuehlwasser:     86C");
  displays[0].setText(1, "Motor Gehaeuse:  42C");
  displays[0].setText(2, "Motoroel:        65C");
  displays[0].setText(3, "Umgebung:        30C");

  displays[1].setText(0, "Luft:            22C");
  displays[1].setText(1, "Wasser:          12C");
  displays[1].setText(2, "Feuchte:         62%");
  displays[1].setText(3, "Druck:      1001 hPa");
  
  displays[2].setText(0, "Battery A:     11.4V");
  displays[2].setText(1, "Battery B:     13.4V");
  displays[2].setText(2, "Verbrauch:     20.4W");
  displays[2].setText(3, "Landstrom:        0V");
  
  displays[3].setText(0, "Diesel:          33%");
  displays[3].setText(1, "Wasser:          92%");
  displays[3].setText(2, "Abwasser:         2%");
  displays[3].setText(3, "Bier:          3/120");
  
  displays[4].setText(0, "Was ist rot und");
  displays[4].setText(1, "wiehert?");
  displays[4].setText(2, "--------------------");
  displays[4].setText(3, "Eine Pferdbeere!");
}

void loop()
{
  refreshDisplays();
}
