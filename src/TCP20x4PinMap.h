#ifndef TCP20X4_PIN_MAP_H
#define TCP20X4_PIN_MAP_H

#include <stdint.h>

struct TCP20x4PinMap {
  uint8_t rs;
  uint8_t rw;
  uint8_t en;
  uint8_t backlight;
  uint8_t d4;
  uint8_t d5;
  uint8_t d6;
  uint8_t d7;
  bool backlightActiveHigh;

  static TCP20x4PinMap CommonYwRobot() {
    TCP20x4PinMap map{};
    map.rs = 0;
    map.rw = 1;
    map.en = 2;
    map.backlight = 3;
    map.d4 = 4;
    map.d5 = 5;
    map.d6 = 6;
    map.d7 = 7;
    map.backlightActiveHigh = true;
    return map;
  }
};

#endif