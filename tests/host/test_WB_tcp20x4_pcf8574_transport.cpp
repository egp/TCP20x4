// tests/host/test_WB_tcp20x4_pcf8574_transport.cpp v1
#include <assert.h>

#include "TCP20x4Pcf8574Transport.h"

static void testCommonYwRobotRowOffsets() {
  const TCP20x4Pcf8574Config config = TCP20x4Pcf8574Config::CommonYwRobot();
  assert(TCP20x4Pcf8574RowAddress(config, 0) == 0x00);
  assert(TCP20x4Pcf8574RowAddress(config, 1) == 0x40);
  assert(TCP20x4Pcf8574RowAddress(config, 2) == 0x14);
  assert(TCP20x4Pcf8574RowAddress(config, 3) == 0x54);
}

static void testComposeNibbleByteForCommonYwRobot() {
  const TCP20x4PinMap pinMap = TCP20x4PinMap::CommonYwRobot();
  const uint8_t value = TCP20x4Pcf8574ComposeNibbleByte(pinMap, true, false, false, 0x0A, true);
  assert(value == 0xA9);
}

static void testComposeNibbleByteRespectsActiveLowBacklight() {
  TCP20x4PinMap pinMap = TCP20x4PinMap::CommonYwRobot();
  pinMap.backlightActiveHigh = false;

  assert(TCP20x4Pcf8574ComposeNibbleByte(pinMap, false, false, false, 0x00, true) == 0x00);
  assert(TCP20x4Pcf8574ComposeNibbleByte(pinMap, false, false, false, 0x00, false) == 0x08);
}

static void testPulseTripletRaisesEnableThenDropsIt() {
  const TCP20x4PinMap pinMap = TCP20x4PinMap::CommonYwRobot();
  const TCP20x4Pcf8574PulseTriplet triplet =
      TCP20x4Pcf8574MakePulseTriplet(pinMap, false, false, 0x03, true);

  assert(triplet.before == 0x38);
  assert(triplet.high == 0x3C);
  assert(triplet.after == 0x38);
}

int main() {
  testCommonYwRobotRowOffsets();
  testComposeNibbleByteForCommonYwRobot();
  testComposeNibbleByteRespectsActiveLowBacklight();
  testPulseTripletRaisesEnableThenDropsIt();
  return 0;
}
// tests/host/test_WB_tcp20x4_pcf8574_transport.cpp v1