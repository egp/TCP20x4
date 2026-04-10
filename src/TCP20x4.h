#ifndef TCP20X4_H
#define TCP20X4_H

#include <Arduino.h>
#include <Wire.h>

#include "TCP20x4Core.h"
#include "TCP20x4PinMap.h"
#include "TCP20x4Status.h"

class TCP20x4 {
 public:
  TCP20x4(TwoWire& wire, uint8_t i2cAddress, const TCP20x4PinMap& pinMap);

  TCP20x4Status begin();
  TCP20x4Status writeLine(uint8_t line, const char* text);
  TCP20x4Status clear();
  TCP20x4Status clearLine(uint8_t line);
  TCP20x4Status displayOn();
  TCP20x4Status displayOff();
  TCP20x4Status backlightOn();
  TCP20x4Status backlightOff();
  TCP20x4Status setBrightness(uint8_t level);

  const TCP20x4Core& core() const;

 private:
  TCP20x4Status syncLineFromCache(uint8_t line);
  TCP20x4Status syncDisplayFromCache();
  TCP20x4Status syncBrightnessFromCache();

  TCP20x4Status sendCommand(uint8_t command);
  TCP20x4Status sendData(uint8_t data);
  TCP20x4Status writeExpander(uint8_t value);

  TwoWire& wire_;
  uint8_t i2cAddress_;
  TCP20x4PinMap pinMap_;
  TCP20x4Core core_;
};

#endif