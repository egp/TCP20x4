// tests/host/test_WB_tcp20x4_core_initial_state.cpp v2
#include <assert.h>
#include <string.h>

#include "TCP20x4Core.h"

static void testInitialState() {
  TCP20x4Core core;

  assert(!core.isInitialized());
  assert(core.isDisplayEnabled());
  assert(core.isBacklightEnabled());
  assert(core.isFullRedrawNeeded());

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(!core.isLineDirty(row));

    const char* line = core.cachedLine(row);
    assert(line != nullptr);
    assert(strlen(line) == TCP20x4Core::kCols);

    for (uint8_t col = 0; col < TCP20x4Core::kCols; ++col) {
      assert(line[col] == ' ');
    }
  }

  assert(core.cachedLine(TCP20x4Core::kRows) == nullptr);
  assert(!core.isLineDirty(TCP20x4Core::kRows));
}

int main() {
  testInitialState();
  return 0;
}
// tests/host/test_WB_tcp20x4_core_initial_state.cpp v2