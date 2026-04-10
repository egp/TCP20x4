// tests/host/test_WB_tcp20x4_core_clear.cpp v2
#include <assert.h>
#include <string.h>

#include "TCP20x4Core.h"

static void testClearLineClearsOnlyTargetLine() {
  TCP20x4Core core;

  assert(core.writeLine(0, "top") == TCP20x4Status::Ok);
  assert(core.writeLine(1, "middle") == TCP20x4Status::Ok);

  assert(core.clearLine(0) == TCP20x4Status::Ok);
  assert(strcmp(core.cachedLine(0), "                    ") == 0);
  assert(strcmp(core.cachedLine(1), "middle              ") == 0);

  assert(core.isLineDirty(0));
  assert(core.isLineDirty(1));
  assert(!core.isLineDirty(2));
  assert(!core.isLineDirty(3));
}

static void testClearLineRejectsInvalidLine() {
  TCP20x4Core core;
  assert(core.clearLine(4) == TCP20x4Status::InvalidLine);
}

static void testClearClearsAllLinesAndMarksDirty() {
  TCP20x4Core core;

  assert(core.writeLine(0, "a") == TCP20x4Status::Ok);
  assert(core.writeLine(1, "b") == TCP20x4Status::Ok);
  assert(core.writeLine(2, "c") == TCP20x4Status::Ok);
  assert(core.writeLine(3, "d") == TCP20x4Status::Ok);

  core.markAllClean();
  assert(core.clear() == TCP20x4Status::Ok);

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(strcmp(core.cachedLine(row), "                    ") == 0);
    assert(core.isLineDirty(row));
  }
}

int main() {
  testClearLineClearsOnlyTargetLine();
  testClearLineRejectsInvalidLine();
  testClearClearsAllLinesAndMarksDirty();
  return 0;
}
// tests/host/test_WB_tcp20x4_core_clear.cpp v2