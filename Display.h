#pragma once

#include <LiquidCrystal_I2C.h>

#define DISPLAY_ADDRESS_0 0x22
#define DISPLAY_ADDRESS_1 0x23
#define DISPLAY_ADDRESS_2 0x25
#define DISPLAY_ADDRESS_3 0x26
#define DISPLAY_ADDRESS_4 0x27

#define DISPLAY_COUNT 5
#define DISPLAY_LINE_COUNT 4
#define DISPLAY_CHAR_COUNT 20
#define DISPLAY_REFRESH_PERIODE 5000

class Display
{
public:
    Display(uint8_t addr) : lcd(addr, DISPLAY_CHAR_COUNT, DISPLAY_LINE_COUNT),
                            lastUpdate(0),
                            needsRefresh(true),
                            lines()
    {
    }

    void setup()
    {
        lcd.init();
        lcd.backlight();
    }

    void refresh()
    {
        if (!needsRefresh)
        {
            return;
        }

        if (millis() - lastUpdate < DISPLAY_REFRESH_PERIODE)
        {
            return;
        }

        for (int i = 0; i < DISPLAY_LINE_COUNT; ++i)
        {
            lcd.setCursor(0, i);
            lcd.print(lines[i]);
        }

        lastUpdate = millis();
        needsRefresh = false;
    }

    void setText(int i, const String &line)
    {
        if (i < 0 || i >= DISPLAY_LINE_COUNT)
        {
            return;
        }

        if (line.length() > DISPLAY_CHAR_COUNT)
        {
            return;
        }

        lines[i] = line;
        needsRefresh = true;
    }

private:
    LiquidCrystal_I2C lcd;
    unsigned long lastUpdate;
    bool needsRefresh;
    String lines[4];
};

Display displays[] = {
    Display(DISPLAY_ADDRESS_0),
    Display(DISPLAY_ADDRESS_1),
    Display(DISPLAY_ADDRESS_2),
    Display(DISPLAY_ADDRESS_3),
    Display(DISPLAY_ADDRESS_4)};

void setupDisplays()
{
    for (int i = 0; i < DISPLAY_COUNT; ++i)
    {
        displays[i].setup();
    }
}

void refreshDisplays()
{
    for (int i = 0; i < DISPLAY_COUNT; ++i)
    {
        displays[i].refresh();
    }
}