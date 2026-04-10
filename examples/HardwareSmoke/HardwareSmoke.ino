// examples/HardwareSmoke/HardwareSmoke.ino v2
#include <Wire.h>

#include <TCP20x4.h>

namespace {

constexpr uint8_t kI2cAddress = 0x27;
TCP20x4 lcd(Wire, kI2cAddress, TCP20x4PinMap::CommonYwRobot());

const char* statusName(TCP20x4Status status) {
  switch (status) {
    case TCP20x4Status::Ok:
      return "Ok";
    case TCP20x4Status::InvalidLine:
      return "InvalidLine";
    case TCP20x4Status::LineTooLong:
      return "LineTooLong";
    case TCP20x4Status::InvalidArgument:
      return "InvalidArgument";
    case TCP20x4Status::NotInitialized:
      return "NotInitialized";
    case TCP20x4Status::TransportError:
      return "TransportError";
    case TCP20x4Status::NotImplemented:
      return "NotImplemented";
    default:
      return "Unknown";
  }
}

void printStatus(const char* label, TCP20x4Status status) {
  Serial.print(label);
  Serial.print(": ");
  Serial.println(statusName(status));
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  printStatus("begin", lcd.begin());
  printStatus("writeLine(0)", lcd.writeLine(0, "stub line 0"));
  printStatus("writeLine(1)", lcd.writeLine(1, "stub line 1"));
  printStatus("displayOff", lcd.displayOff());
  printStatus("displayOn", lcd.displayOn());
  printStatus("backlightOff", lcd.backlightOff());
  printStatus("backlightOn", lcd.backlightOn());
}

void loop() {
}
// examples/HardwareSmoke/HardwareSmoke.ino v2