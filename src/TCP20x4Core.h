// src/TCP20x4Core.h v2
#ifndef TCP20X4_CORE_H
#define TCP20X4_CORE_H

#include <stdint.h>

#include "TCP20x4Status.h"

class TCP20x4Core {
public:
  static constexpr uint8_t kRows = 4;
  static constexpr uint8_t kCols = 20;

  TCP20x4Core();

  TCP20x4Status initialize();
  TCP20x4Status writeLine(uint8_t line, const char* text);
  TCP20x4Status clear();
  TCP20x4Status clearLine(uint8_t line);
  TCP20x4Status displayOn();
  TCP20x4Status displayOff();
  TCP20x4Status backlightOn();
  TCP20x4Status backlightOff();

  bool isInitialized() const;
  bool isDisplayEnabled() const;
  bool isBacklightEnabled() const;
  const char* cachedLine(uint8_t line) const;
  bool isLineDirty(uint8_t line) const;
  bool isFullRedrawNeeded() const;

  void markLineClean(uint8_t line);
  void markAllClean();

private:
  char cache_[kRows][kCols + 1];
  bool initialized_;
  bool displayEnabled_;
  bool backlightEnabled_;
  bool lineDirty_[kRows];
  bool fullRedrawNeeded_;
};

#endif
// src/TCP20x4Core.h v2