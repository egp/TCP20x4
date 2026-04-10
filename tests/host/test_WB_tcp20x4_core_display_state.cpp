// tests/host/test_tcp20x4_core_display_state.cpp v1
#include <assert.h>
#include <string.h>

#include "TCP20x4Core.h"

static void testInitializeMarksCoreInitializedAndDirty() {
  TCP20x4Core core;

  core.markAllClean();

  assert(core.initialize() == TCP20x4Status::Ok);
  assert(core.isInitialized());
  assert(core.isDisplayEnabled());
  assert(core.isFullRedrawNeeded());

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(core.isLineDirty(row));
    assert(strcmp(core.cachedLine(row), "                    ") == 0);
  }
}

static void testDisplayOffThenOnForcesRedrawFromCache() {
  TCP20x4Core core;

  assert(core.writeLine(2, "cache survives off") == TCP20x4Status::Ok);
  core.markAllClean();

  assert(core.displayOff() == TCP20x4Status::Ok);
  assert(!core.isDisplayEnabled());
  assert(strcmp(core.cachedLine(2), "cache survives off  ") == 0);

  assert(core.displayOn() == TCP20x4Status::Ok);
  assert(core.isDisplayEnabled());
  assert(core.isFullRedrawNeeded());

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(core.isLineDirty(row));
  }

  assert(strcmp(core.cachedLine(2), "cache survives off  ") == 0);
}

static void testSetBrightnessCachesLevel() {
  TCP20x4Core core;

  assert(core.setBrightness(0) == TCP20x4Status::Ok);
  assert(core.brightness() == 0);

  assert(core.setBrightness(17) == TCP20x4Status::Ok);
  assert(core.brightness() == 17);

  assert(core.setBrightness(255) == TCP20x4Status::Ok);
  assert(core.brightness() == 255);
}

int main() {
  testInitializeMarksCoreInitializedAndDirty();
  testDisplayOffThenOnForcesRedrawFromCache();
  testSetBrightnessCachesLevel();
  return 0;
}
// tests/host/test_tcp20x4_core_display_state.cpp v1