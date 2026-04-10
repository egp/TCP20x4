// src/TCP20x4Pcf8574Transport.h v1
#ifndef TCP20X4_PCF8574_TRANSPORT_H
#define TCP20X4_PCF8574_TRANSPORT_H

#include <stdint.h>

#include "TCP20x4Pcf8574Config.h"

struct TCP20x4Pcf8574PulseTriplet {
  uint8_t before;
  uint8_t high;
  uint8_t after;
};

uint8_t TCP20x4Pcf8574RowAddress(const TCP20x4Pcf8574Config& config, uint8_t line);

uint8_t TCP20x4Pcf8574ComposeNibbleByte(const TCP20x4PinMap& pinMap,
                                        bool rs,
                                        bool rw,
                                        bool en,
                                        uint8_t nibble,
                                        bool backlightEnabled);

TCP20x4Pcf8574PulseTriplet TCP20x4Pcf8574MakePulseTriplet(const TCP20x4PinMap& pinMap,
                                                          bool rs,
                                                          bool rw,
                                                          uint8_t nibble,
                                                          bool backlightEnabled);

#endif
// src/TCP20x4Pcf8574Transport.h v1