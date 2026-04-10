// src/TCP20x4Controller.h v1
#ifndef TCP20X4_CONTROLLER_H
#define TCP20X4_CONTROLLER_H

#include <stdint.h>

#include "TCP20x4Core.h"
#include "TCP20x4Device.h"
#include "TCP20x4Status.h"

class TCP20x4Controller {
 public:
  explicit TCP20x4Controller(TCP20x4Device& device);

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

  TCP20x4Device& device_;
  TCP20x4Core core_;
};

#endif
// src/TCP20x4Controller.h v1