#pragma once

#include "Wire.h"

class TCA {
 public:
  TCA(uint8_t addr, int reset_pin) : address(addr), reset_pin(reset_pin) {
    pinMode(reset_pin, OUTPUT);
    disable();
  }

  void setTCAChannel(uint8_t channel) {
    Wire.beginTransmission(address);
    Wire.write(1 << channel);
    Wire.endTransmission();
  }

  void disable() { digitalWrite(reset_pin, LOW); }
  void enable() { digitalWrite(reset_pin, HIGH); }

 private:
  uint8_t address;
  int reset_pin;
};
