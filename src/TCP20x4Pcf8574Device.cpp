// src/TCP20x4Pcf8574Device.cpp v2
#include "TCP20x4Pcf8574Device.h"

#include "TCP20x4Core.h"
#include "TCP20x4Pcf8574Transport.h"

namespace {
uint8_t backlightMask(const TCP20x4PinMap& pinMap) {
  return static_cast<uint8_t>(1u << pinMap.backlight);
}
}  // namespace

TCP20x4Pcf8574Device::TCP20x4Pcf8574Device(
    BBI2C& i2c,
    const TCP20x4Pcf8574Config& config)
    : i2c_(i2c),
      config_(config),
      portState_(0),
      initialized_(false),
      backlightEnabled_(true) {}

TCP20x4Status TCP20x4Pcf8574Device::initialize() {
  initialized_ = false;
  backlightEnabled_ = true;
  portState_ = applyBacklightToIdleByte(0);

  delay(50);

  TCP20x4Status status = writeNibble(false, 0x03);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  delayMicroseconds(4500);

  status = writeNibble(false, 0x03);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  delayMicroseconds(4500);

  status = writeNibble(false, 0x03);
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  delayMicroseconds(150);

  status = writeNibble(false, 0x02);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = command(0x28);  // 4-bit, 2-line, 5x8 dots
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = command(0x08);  // display off
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = command(0x01);  // clear
  if (status != TCP20x4Status::Ok) {
    return status;
  }
  delayMicroseconds(2000);

  status = command(0x06);  // entry mode set
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = command(0x0C);  // display on, cursor off, blink off
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  initialized_ = true;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::setDisplayEnabled(bool enabled) {
  if (!initialized_) {
    return TCP20x4Status::NotInitialized;
  }
  return command(enabled ? 0x0C : 0x08);
}

TCP20x4Status TCP20x4Pcf8574Device::setBacklightEnabled(bool enabled) {
  if (!initialized_) {
    return TCP20x4Status::NotInitialized;
  }

  backlightEnabled_ = enabled;
  return writePort(applyBacklightToIdleByte(portState_));
}

TCP20x4Status TCP20x4Pcf8574Device::writeLine(uint8_t line, const char* text) {
  if (!initialized_) {
    return TCP20x4Status::NotInitialized;
  }
  if (line >= TCP20x4Core::kRows) {
    return TCP20x4Status::InvalidLine;
  }
  if (text == nullptr) {
    return TCP20x4Status::InvalidArgument;
  }

  TCP20x4Status status = setCursor(line, 0);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  for (uint8_t col = 0; col < TCP20x4Core::kCols; ++col) {
    const char ch = (text[col] == '\0') ? ' ' : text[col];
    status = data(static_cast<uint8_t>(ch));
    if (status != TCP20x4Status::Ok) {
      return status;
    }
  }

  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::writePort(uint8_t value) {
  uint8_t data[1] = {value};
  const int written = I2CWrite(&i2c_, config_.address, data, 1);
  if (written != 1) {
    return TCP20x4Status::TransportError;
  }

  portState_ = value;
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::writeNibble(bool rs, uint8_t nibble) {
  const TCP20x4Pcf8574PulseTriplet triplet =
      TCP20x4Pcf8574MakePulseTriplet(
          config_.pinMap,
          rs,
          false,
          static_cast<uint8_t>(nibble & 0x0F),
          backlightEnabled_);

  TCP20x4Status status = writePort(triplet.before);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = writePort(triplet.high);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = writePort(triplet.after);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  delayMicroseconds(1);
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::writeByte(bool rs, uint8_t value) {
  TCP20x4Status status = writeNibble(rs, static_cast<uint8_t>((value >> 4) & 0x0F));
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  status = writeNibble(rs, static_cast<uint8_t>(value & 0x0F));
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  delayMicroseconds(40);
  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::command(uint8_t value) {
  const TCP20x4Status status = writeByte(false, value);
  if (status != TCP20x4Status::Ok) {
    return status;
  }

  if (value == 0x01 || value == 0x02) {
    delayMicroseconds(2000);
  }

  return TCP20x4Status::Ok;
}

TCP20x4Status TCP20x4Pcf8574Device::data(uint8_t value) {
  return writeByte(true, value);
}

TCP20x4Status TCP20x4Pcf8574Device::setCursor(uint8_t line, uint8_t column) {
  if (line >= TCP20x4Core::kRows || column >= TCP20x4Core::kCols) {
    return TCP20x4Status::InvalidArgument;
  }

  const uint8_t address =
      static_cast<uint8_t>(TCP20x4Pcf8574RowAddress(config_, line) + column);
  return command(static_cast<uint8_t>(0x80 | address));
}

uint8_t TCP20x4Pcf8574Device::applyBacklightToIdleByte(uint8_t value) const {
  if (backlightEnabled_ == config_.pinMap.backlightActiveHigh) {
    value = static_cast<uint8_t>(value | backlightMask(config_.pinMap));
  } else {
    value = static_cast<uint8_t>(value & static_cast<uint8_t>(~backlightMask(config_.pinMap)));
  }
  return value;
}
// src/TCP20x4Pcf8574Device.cpp v2