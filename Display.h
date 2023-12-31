#pragma once

#include <LiquidCrystal_I2C.h>

#define DISPLAY_COUNT 5
#define DISPLAY_LINE_COUNT 4
#define DISPLAY_CHAR_COUNT 20
#define DISPLAY_UPDATE_PERIODE 5000

class Display {
 public:
  Display(uint8_t addr, TCA& tca)
      : is_setup(false),
        tca(tca),
        lcd(addr, DISPLAY_CHAR_COUNT, DISPLAY_LINE_COUNT),
        lastUpdate(0),
        needsRefresh(),
        lines() {
    for (int i = 0; i < DISPLAY_LINE_COUNT; ++i) {
      setText(i, "");
    }
  }

  void reset() {
    is_setup = false;
    for (int i = 0; i < DISPLAY_LINE_COUNT; ++i) {
      needsRefresh[i] = true;
    }
  }

  void setup() {
    if (is_setup) {
      return;
    }

    tca.setTCAChannel(TCA_DISPLAY_CHANNEL);

    lcd.init();
    lcd.backlight();

    is_setup = true;
  }

  void update() {
    if (!is_setup) {
      return;
    }

    if (millis() - lastUpdate < DISPLAY_UPDATE_PERIODE) {
      return;
    }

    tca.setTCAChannel(TCA_DISPLAY_CHANNEL);

    for (int i = 0; i < DISPLAY_LINE_COUNT; ++i) {
      if (!needsRefresh[i]) {
        continue;
      }

      lcd.setCursor(0, i);
      lcd.print(lines[i]);
      needsRefresh[i] = false;
    }

    lastUpdate = millis();
  }

  void setText(int i, const String &line) {
    if (i < 0 || i >= DISPLAY_LINE_COUNT) {
      return;
    }

    lines[i] = line.substring(0, DISPLAY_CHAR_COUNT);
    needsRefresh[i] = true;
  }

 private:
  bool is_setup;
  TCA& tca;
  LiquidCrystal_I2C lcd;
  unsigned long lastUpdate;
  bool needsRefresh[DISPLAY_LINE_COUNT];
  String lines[DISPLAY_LINE_COUNT];
};
