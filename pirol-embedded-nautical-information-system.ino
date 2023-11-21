#include <LiquidCrystal_I2C.h>

#define DISPLAY_ADDRESS_0 0x22
#define DISPLAY_ADDRESS_1 0x23
#define DISPLAY_ADDRESS_2 0x25
#define DISPLAY_ADDRESS_3 0x26
#define DISPLAY_ADDRESS_4 0x27

#define DISPLAY_COUNT 5
#define DISPLAY_REFRESH_PERIODE 5000

class Display
{
public:
  Display(uint8_t addr) : lcd(addr, 20, 4),
                          lastUpdate(0),
                          needsRefresh(true),
                          line()
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

    lcd.setCursor(0, 0);
    lcd.print(line);

    lastUpdate = millis();
    needsRefresh = false;
  }

  void setText(const String &line)
  {
    line = line;
    needsRefresh = true;
  }

private:
  LiquidCrystal_I2C lcd;
  unsigned long lastUpdate;
  bool needsRefresh;
  String line;
};

Display displays[] = {
    Display(DISPLAY_ADDRESS_0),
    Display(DISPLAY_ADDRESS_1),
    Display(DISPLAY_ADDRESS_2),
    Display(DISPLAY_ADDRESS_3),
    Display(DISPLAY_ADDRESS_4)};

void setup()
{
  Serial.begin(9600);

  delay(500);

  for (int i = 0; i < DISPLAY_COUNT; ++i)
  {
    displays[i].setup();
  }

  // Switch to I2C fast mode
  Wire.setClock(400000);

  displays[0].setText("A");
  displays[1].setText("B");
  displays[2].setText("C");
  displays[3].setText("1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  displays[4].setText("Hi? Ho! 2^3 30 C 123 RPM");
}

void loop()
{
  for (int i = 0; i < DISPLAY_COUNT; ++i)
  {
    displays[i].refresh();
  }
}
