// tests/host/test_WB_tcp20x4_core_display_state.cpp v2
#include <assert.h>
#include <string.h>

#include "TCP20x4Core.h"

static void testInitializeMarksCoreInitializedAndDirty() {
  TCP20x4Core core;
  core.markAllClean();

  assert(core.initialize() == TCP20x4Status::Ok);
  assert(core.isInitialized());
  assert(core.isDisplayEnabled());
  assert(core.isBacklightEnabled());
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

static void testBacklightStateCachesOnOff() {
  TCP20x4Core core;

  assert(core.isBacklightEnabled());
  assert(core.backlightOff() == TCP20x4Status::Ok);
  assert(!core.isBacklightEnabled());
  assert(core.backlightOn() == TCP20x4Status::Ok);
  assert(core.isBacklightEnabled());
}

int main() {
  testInitializeMarksCoreInitializedAndDirty();
  testDisplayOffThenOnForcesRedrawFromCache();
  testBacklightStateCachesOnOff();
  return 0;
}
// tests/host/test_WB_tcp20x4_core_display_state.cpp v2