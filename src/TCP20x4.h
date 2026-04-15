// src/TCP20x4.h v4
#ifndef TCP20X4_H
#define TCP20X4_H

#include <Arduino.h>
#include <TCP1819.h>

#include "TCP20x4Controller.h"
#include "TCP20x4Core.h"
#include "TCP20x4Pcf8574Config.h"
#include "TCP20x4Pcf8574Device.h"
#include "TCP20x4PinMap.h"
#include "TCP20x4Status.h"

class TCP20x4 {
public:
  TCP20x4(BBI2C& i2c, const TCP20x4Pcf8574Config& config);
  TCP20x4(BBI2C& i2c, uint8_t i2cAddress, const TCP20x4PinMap& pinMap);

  TCP20x4Status begin();
  TCP20x4Status writeLine(uint8_t line, const char* text);
  TCP20x4Status clear();
  TCP20x4Status clearLine(uint8_t line);
  TCP20x4Status displayOn();
  TCP20x4Status displayOff();
  TCP20x4Status backlightOn();
  TCP20x4Status backlightOff();

  const TCP20x4Core& core() const;

private:
  TCP20x4Pcf8574Device device_;
  TCP20x4Controller controller_;
};

#endif
// src/TCP20x4.h v4