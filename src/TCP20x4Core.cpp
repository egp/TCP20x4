// src/TCP20x4Core.cpp v5
#include "TCP20x4Core.h"

namespace {

void fillLineWithSpaces(char* line, uint8_t cols) {
  for (uint8_t i = 0; i < cols; ++i) {
    line[i] = ' ';
  }
  line[cols] = '\0';
}

TCP20x4Status validateAndMeasureLineText(const char* text, uint8_t maxLen, uint8_t* outLen) {
  if (text == nullptr || outLen == nullptr) {
    return TCP20x4Status::InvalidArgument;
  }

  uint8_t len = 0;
  while (text[len] != '\0') {
    if (len >= maxLen) {
      return TCP20x4Status::LineTooLong;
    }
    ++len;
  }

  *outLen = len;
  return TCP20x4Status::Ok;
}

}  // namespace

TCP20x4Core::TCP20x4Core()
    : initialized_(false),
      displayEnabled_(true),
      backlightEnabled_(true),
      fullRedrawNeeded_(true) {
  for (uint8_t row = 0; row < kRows; ++row) {
    fillLineWithSpaces(cache_[row], kCols);
    lineDirty_[row] = false;
  }
}

TCP20x4Status TCP20x4Core::initialize() {
  initialized_ = true;
  displayEnabled_ = true;
  backlightEnabled_ = true;
  fullRedrawNeeded_ = true;

  for (uint8_t row = 0; row < kRows; ++row) {
    lineDirty_[row] = true;
  }

  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::writeLine(uint8_t line, const char* text) {
  if (line >= kRows) {
    return TCP20x4Status::InvalidLine;
  }

  uint8_t textLen = 0;
  const TCP20x4Status status = validateAndMeasureLineText(text, kCols, &textLen);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  fillLineWithSpaces(cache_[line], kCols);
  for (uint8_t i = 0; i < textLen; ++i) {
    cache_[line][i] = text[i];
  }

  lineDirty_[line] = true;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::clear() {
  for (uint8_t row = 0; row < kRows; ++row) {
    fillLineWithSpaces(cache_[row], kCols);
    lineDirty_[row] = true;
  }
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::clearLine(uint8_t line) {
  if (line >= kRows) {
    return TCP20x4Status::InvalidLine;
  }

  fillLineWithSpaces(cache_[line], kCols);
  lineDirty_[line] = true;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::displayOn() {
  displayEnabled_ = true;
  fullRedrawNeeded_ = true;

  for (uint8_t row = 0; row < kRows; ++row) {
    lineDirty_[row] = true;
  }

  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::displayOff() {
  displayEnabled_ = false;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::backlightOn() {
  backlightEnabled_ = true;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Core::backlightOff() {
  backlightEnabled_ = false;
  return TCP20x4Status::Ok;
}

bool TCP20x4Core::isInitialized() const {
  return initialized_;
}

bool TCP20x4Core::isDisplayEnabled() const {
  return displayEnabled_;
}

bool TCP20x4Core::isBacklightEnabled() const {
  return backlightEnabled_;
}

const char* TCP20x4Core::cachedLine(uint8_t line) const {
  if (line >= kRows) {
    return nullptr;
  }
  return cache_[line];
}

bool TCP20x4Core::isLineDirty(uint8_t line) const {
  if (line >= kRows) {
    return false;
  }
  return lineDirty_[line];
}

bool TCP20x4Core::isFullRedrawNeeded() const {
  return fullRedrawNeeded_;
}

void TCP20x4Core::markLineClean(uint8_t line) {
  if (line >= kRows) {
    return;
  }
  lineDirty_[line] = false;
}

void TCP20x4Core::markAllClean() {
  for (uint8_t row = 0; row < kRows; ++row) {
    lineDirty_[row] = false;
  }
  fullRedrawNeeded_ = false;
}
// src/TCP20x4Core.cpp v5