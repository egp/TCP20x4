// src/TCP20x4Device.h v2
#ifndef TCP20X4_DEVICE_H
#define TCP20X4_DEVICE_H

#include <stdint.h>

#include "TCP20x4Status.h"

class TCP20x4Device {
public:
  virtual ~TCP20x4Device() = default;

  virtual TCP20x4Status initialize() = 0;
  virtual TCP20x4Status setDisplayEnabled(bool enabled) = 0;
  virtual TCP20x4Status setBacklightEnabled(bool enabled) = 0;
  virtual TCP20x4Status writeLine(uint8_t line, const char* text) = 0;
};

#endif
// src/TCP20x4Device.h v2