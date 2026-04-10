// src/TCP20x4.cpp v3
#include "TCP20x4.h"

TCP20x4::TCP20x4(TwoWire& wire, const TCP20x4Pcf8574Config& config)
    : device_(wire, config),
      controller_(device_) {}

TCP20x4::TCP20x4(TwoWire& wire, uint8_t i2cAddress, const TCP20x4PinMap& pinMap)
    : TCP20x4(wire, TCP20x4Pcf8574Config::FromAddressAndPinMap(i2cAddress, pinMap)) {}

TCP20x4Status TCP20x4::begin() {
  return controller_.begin();
}

TCP20x4Status TCP20x4::writeLine(uint8_t line, const char* text) {
  return controller_.writeLine(line, text);
}

TCP20x4Status TCP20x4::clear() {
  return controller_.clear();
}

TCP20x4Status TCP20x4::clearLine(uint8_t line) {
  return controller_.clearLine(line);
}

TCP20x4Status TCP20x4::displayOn() {
  return controller_.displayOn();
}

TCP20x4Status TCP20x4::displayOff() {
  return controller_.displayOff();
}

TCP20x4Status TCP20x4::backlightOn() {
  return controller_.backlightOn();
}

TCP20x4Status TCP20x4::backlightOff() {
  return controller_.backlightOff();
}

const TCP20x4Core& TCP20x4::core() const {
  return controller_.core();
}
// src/TCP20x4.cpp v3