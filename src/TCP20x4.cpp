#include "TCP20x4.h"

TCP20x4::TCP20x4(TwoWire& wire, uint8_t i2cAddress, const TCP20x4PinMap& pinMap)
    : wire_(wire),
      i2cAddress_(i2cAddress),
      pinMap_(pinMap),
      core_() {}

TCP20x4Status TCP20x4::begin() {
  wire_.begin();
  return core_.initialize();
}

TCP20x4Status TCP20x4::writeLine(uint8_t line, const char* text) {
  const TCP20x4Status status = core_.writeLine(line, text);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return syncLineFromCache(line);
}

TCP20x4Status TCP20x4::clear() {
  const TCP20x4Status status = core_.clear();
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return syncDisplayFromCache();
}

TCP20x4Status TCP20x4::clearLine(uint8_t line) {
  const TCP20x4Status status = core_.clearLine(line);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return syncLineFromCache(line);
}

TCP20x4Status TCP20x4::displayOn() {
  const TCP20x4Status status = core_.displayOn();
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return syncDisplayFromCache();
}

TCP20x4Status TCP20x4::displayOff() {
  const TCP20x4Status status = core_.displayOff();
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return sendCommand(0x08);
}

TCP20x4Status TCP20x4::backlightOn() {
  return setBrightness(255);
}

TCP20x4Status TCP20x4::backlightOff() {
  return setBrightness(0);
}

TCP20x4Status TCP20x4::setBrightness(uint8_t level) {
  const TCP20x4Status status = core_.setBrightness(level);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  return syncBrightnessFromCache();
}

const TCP20x4Core& TCP20x4::core() const {
  return core_;
}

TCP20x4Status TCP20x4::syncLineFromCache(uint8_t line) {
  (void)line;
  return TCP20x4Status::NotImplemented;
}

TCP20x4Status TCP20x4::syncDisplayFromCache() {
  return TCP20x4Status::NotImplemented;
}

TCP20x4Status TCP20x4::syncBrightnessFromCache() {
  return TCP20x4Status::NotImplemented;
}

TCP20x4Status TCP20x4::sendCommand(uint8_t command) {
  (void)command;
  return TCP20x4Status::NotImplemented;
}

TCP20x4Status TCP20x4::sendData(uint8_t data) {
  (void)data;
  return TCP20x4Status::NotImplemented;
}

TCP20x4Status TCP20x4::writeExpander(uint8_t value) {
  (void)value;
  return TCP20x4Status::NotImplemented;
}