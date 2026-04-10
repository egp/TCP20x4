// src/TCP20x4Pcf8574Config.h v1
#ifndef TCP20X4_PCF8574_CONFIG_H
#define TCP20X4_PCF8574_CONFIG_H

#include <stdint.h>

#include "TCP20x4PinMap.h"

struct TCP20x4Pcf8574Config {
  uint8_t address;
  TCP20x4PinMap pinMap;
  uint8_t rowOffsets[4];

  static TCP20x4Pcf8574Config FromAddressAndPinMap(uint8_t address, const TCP20x4PinMap& pinMap) {
    TCP20x4Pcf8574Config config{};
    config.address = address;
    config.pinMap = pinMap;
    config.rowOffsets[0] = 0x00;
    config.rowOffsets[1] = 0x40;
    config.rowOffsets[2] = 0x14;
    config.rowOffsets[3] = 0x54;
    return config;
  }

  static TCP20x4Pcf8574Config CommonYwRobot(uint8_t address = 0x27) {
    return FromAddressAndPinMap(address, TCP20x4PinMap::CommonYwRobot());
  }
};

#endif
// src/TCP20x4Pcf8574Config.h v1