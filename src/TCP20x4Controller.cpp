// src/TCP20x4Controller.cpp v1
#include "TCP20x4Controller.h"

TCP20x4Controller::TCP20x4Controller(TCP20x4Device& device)
    : device_(device), core_() {}

TCP20x4Status TCP20x4Controller::begin() {
  TCP20x4Status status = device_.initialize();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = core_.initialize();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = syncBrightnessFromCache();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = device_.setDisplayEnabled(core_.isDisplayEnabled());
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  return syncDisplayFromCache();
}

TCP20x4Status TCP20x4Controller::writeLine(uint8_t line, const char* text) {
  const TCP20x4Status status = core_.writeLine(line, text);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  if (!core_.isDisplayEnabled()) {
    return TCP20x4Status::Ok;
  }

  return syncLineFromCache(line);
}

TCP20x4Status TCP20x4Controller::clear() {
  const TCP20x4Status status = core_.clear();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  if (!core_.isDisplayEnabled()) {
    return TCP20x4Status::Ok;
  }

  return syncDisplayFromCache();
}

TCP20x4Status TCP20x4Controller::clearLine(uint8_t line) {
  const TCP20x4Status status = core_.clearLine(line);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  if (!core_.isDisplayEnabled()) {
    return TCP20x4Status::Ok;
  }

  return syncLineFromCache(line);
}

TCP20x4Status TCP20x4Controller::displayOn() {
  const TCP20x4Status status = core_.displayOn();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  TCP20x4Status hwStatus = device_.setDisplayEnabled(true);
  if (hwStatus != TCP20x4Status::Ok) {
    return hwStatus;
  }

  return syncDisplayFromCache();
}

TCP20x4Status TCP20x4Controller::displayOff() {
  const TCP20x4Status status = core_.displayOff();
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  return device_.setDisplayEnabled(false);
}

TCP20x4Status TCP20x4Controller::backlightOn() {
  return setBrightness(255);
}

TCP20x4Status TCP20x4Controller::backlightOff() {
  return setBrightness(0);
}

TCP20x4Status TCP20x4Controller::setBrightness(uint8_t level) {
  const TCP20x4Status status = core_.setBrightness(level);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  return syncBrightnessFromCache();
}

const TCP20x4Core& TCP20x4Controller::core() const {
  return core_;
}

TCP20x4Status TCP20x4Controller::syncLineFromCache(uint8_t line) {
  const char* text = core_.cachedLine(line);
  if (text == nullptr) {
    return TCP20x4Status::InvalidLine;
  }

  const TCP20x4Status status = device_.writeLine(line, text);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  core_.markLineClean(line);
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Controller::syncDisplayFromCache() {
  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    const TCP20x4Status status = device_.writeLine(row, core_.cachedLine(row));
    if (status != TCP20x4Status::Ok) {
      return status;
    }
  }

  core_.markAllClean();
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Controller::syncBrightnessFromCache() {
  return device_.setBrightness(core_.brightness());
}
// src/TCP20x4Controller.cpp v1