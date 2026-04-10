// src/TCP20x4Pcf8574Device.h v1
#ifndef TCP20X4_PCF8574_DEVICE_H
#define TCP20X4_PCF8574_DEVICE_H

#include <Arduino.h>
#include <Wire.h>

#include "TCP20x4Device.h"
#include "TCP20x4Pcf8574Config.h"

class TCP20x4Pcf8574Device : public TCP20x4Device {
public:
  TCP20x4Pcf8574Device(TwoWire& wire, const TCP20x4Pcf8574Config& config);

  TCP20x4Status initialize() override;
  TCP20x4Status setDisplayEnabled(bool enabled) override;
  TCP20x4Status setBacklightEnabled(bool enabled) override;
  TCP20x4Status writeLine(uint8_t line, const char* text) override;

private:
  TCP20x4Status writePort(uint8_t value);
  TCP20x4Status writeNibble(bool rs, uint8_t nibble);
  TCP20x4Status writeByte(bool rs, uint8_t value);
  TCP20x4Status command(uint8_t value);
  TCP20x4Status data(uint8_t value);
  TCP20x4Status setCursor(uint8_t line, uint8_t column);
  uint8_t applyBacklightToIdleByte(uint8_t value) const;

  TwoWire& wire_;
  TCP20x4Pcf8574Config config_;
  uint8_t portState_;
  bool initialized_;
  bool backlightEnabled_;
};

#endif
// src/TCP20x4Pcf8574Device.h v1