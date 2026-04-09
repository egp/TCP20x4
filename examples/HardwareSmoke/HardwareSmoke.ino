#include <Wire.h>

namespace {
constexpr uint8_t kPossibleAddresses[] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                                          0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F};

bool i2cPresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

int8_t findBackpack() {
  for (uint8_t i = 0; i < sizeof(kPossibleAddresses); ++i) {
    if (i2cPresent(kPossibleAddresses[i])) {
      return static_cast<int8_t>(kPossibleAddresses[i]);
    }
  }
  return -1;
}
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  delay(200);
  Serial.println();
  Serial.println("TCP20x4 HardwareSmoke");
  Serial.println("Scanning for PCF8574 / PCF8574A backpack...");

  const int8_t found = findBackpack();
  if (found < 0) {
    Serial.println("No backpack found at 0x20..0x27 or 0x38..0x3F");
    return;
  }

  Serial.print("Found backpack at 0x");
  if (found < 16) {
    Serial.print('0');
  }
  Serial.println(found, HEX);

  Serial.println("Next step: add raw port write test and LCD init test.");
}

void loop() {
}