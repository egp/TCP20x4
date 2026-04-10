// src/TCP20x4Pcf8574Transport.cpp v1
#include "TCP20x4Pcf8574Transport.h"

namespace {

uint8_t bitForPin(uint8_t pin) {
  return static_cast<uint8_t>(1u << pin);
}

}  // namespace

uint8_t TCP20x4Pcf8574RowAddress(const TCP20x4Pcf8574Config& config, uint8_t line) {
  return line < 4 ? config.rowOffsets[line] : 0x00;
}

uint8_t TCP20x4Pcf8574ComposeNibbleByte(const TCP20x4PinMap& pinMap,
                                        bool rs,
                                        bool rw,
                                        bool en,
                                        uint8_t nibble,
                                        bool backlightEnabled) {
  uint8_t value = 0;

  if (rs) {
    value |= bitForPin(pinMap.rs);
  }
  if (rw) {
    value |= bitForPin(pinMap.rw);
  }
  if (en) {
    value |= bitForPin(pinMap.en);
  }

  if (backlightEnabled == pinMap.backlightActiveHigh) {
    value |= bitForPin(pinMap.backlight);
  }

  if (nibble & 0x01) {
    value |= bitForPin(pinMap.d4);
  }
  if (nibble & 0x02) {
    value |= bitForPin(pinMap.d5);
  }
  if (nibble & 0x04) {
    value |= bitForPin(pinMap.d6);
  }
  if (nibble & 0x08) {
    value |= bitForPin(pinMap.d7);
  }

  return value;
}

TCP20x4Pcf8574PulseTriplet TCP20x4Pcf8574MakePulseTriplet(const TCP20x4PinMap& pinMap,
                                                          bool rs,
                                                          bool rw,
                                                          uint8_t nibble,
                                                          bool backlightEnabled) {
  TCP20x4Pcf8574PulseTriplet triplet{};
  triplet.before = TCP20x4Pcf8574ComposeNibbleByte(pinMap, rs, rw, false, nibble, backlightEnabled);
  triplet.high = TCP20x4Pcf8574ComposeNibbleByte(pinMap, rs, rw, true, nibble, backlightEnabled);
  triplet.after = triplet.before;
  return triplet;
}
// src/TCP20x4Pcf8574Transport.cpp v1