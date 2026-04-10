// tests/host/test_WB_tcp20x4_core_write_line.cpp v2
#include <assert.h>
#include <string.h>

#include "TCP20x4Core.h"

static void testWriteLinePadsShortInputTo20Columns() {
  TCP20x4Core core;

  const TCP20x4Status status = core.writeLine(0, "abc");
  assert(status == TCP20x4Status::Ok);

  const char* line0 = core.cachedLine(0);
  assert(line0 != nullptr);
  assert(strlen(line0) == TCP20x4Core::kCols);
  assert(strcmp(line0, "abc                 ") == 0);

  assert(core.isLineDirty(0));
  assert(!core.isLineDirty(1));
  assert(!core.isLineDirty(2));
  assert(!core.isLineDirty(3));
}

static void testWriteLineRejectsTooLongInputWithoutPartialUpdate() {
  TCP20x4Core core;

  const TCP20x4Status first = core.writeLine(1, "short");
  assert(first == TCP20x4Status::Ok);
  assert(strcmp(core.cachedLine(1), "short               ") == 0);

  const TCP20x4Status second = core.writeLine(1, "123456789012345678901");
  assert(second == TCP20x4Status::LineTooLong);
  assert(strcmp(core.cachedLine(1), "short               ") == 0);
}

static void testWriteLineRejectsInvalidLine() {
  TCP20x4Core core;
  assert(core.writeLine(4, "abc") == TCP20x4Status::InvalidLine);
}

static void testWriteLineRejectsNullText() {
  TCP20x4Core core;
  assert(core.writeLine(0, nullptr) == TCP20x4Status::InvalidArgument);
}

int main() {
  testWriteLinePadsShortInputTo20Columns();
  testWriteLineRejectsTooLongInputWithoutPartialUpdate();
  testWriteLineRejectsInvalidLine();
  testWriteLineRejectsNullText();
  return 0;
}
// tests/host/test_WB_tcp20x4_core_write_line.cpp v2