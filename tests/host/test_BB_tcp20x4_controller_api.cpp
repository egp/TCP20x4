// tests/host/test_BB_tcp20x4_controller_api.cpp v1
#include <assert.h>
#include <string.h>

#include <string>
#include <vector>

#include "TCP20x4Controller.h"

struct WriteRecord {
  uint8_t line;
  std::string text;
};

class FakeDevice : public TCP20x4Device {
 public:
  TCP20x4Status initialize() override {
    ++initializeCount;
    return TCP20x4Status::Ok;
  }

  TCP20x4Status setDisplayEnabled(bool enabled) override {
    displayCalls.push_back(enabled ? 1 : 0);
    return TCP20x4Status::Ok;
  }

  TCP20x4Status setBrightness(uint8_t level) override {
    brightnessCalls.push_back(level);
    return TCP20x4Status::Ok;
  }

  TCP20x4Status writeLine(uint8_t line, const char* text) override {
    writes.push_back({line, text == nullptr ? std::string() : std::string(text)});
    return TCP20x4Status::Ok;
  }

  void resetLogs() {
    displayCalls.clear();
    brightnessCalls.clear();
    writes.clear();
  }

  int initializeCount = 0;
  std::vector<int> displayCalls;
  std::vector<uint8_t> brightnessCalls;
  std::vector<WriteRecord> writes;
};

static void testBeginSyncsBlankDisplayAndDefaults() {
  FakeDevice device;
  TCP20x4Controller controller(device);

  assert(controller.begin() == TCP20x4Status::Ok);

  assert(device.initializeCount == 1);
  assert(device.displayCalls.size() == 1);
  assert(device.displayCalls[0] == 1);
  assert(device.brightnessCalls.size() == 1);
  assert(device.brightnessCalls[0] == 255);
  assert(device.writes.size() == TCP20x4Core::kRows);

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(device.writes[row].line == row);
    assert(device.writes[row].text == "                    ");
    assert(!controller.core().isLineDirty(row));
  }
}

static void testWriteLineWritesThroughCacheWhenVisible() {
  FakeDevice device;
  TCP20x4Controller controller(device);

  assert(controller.begin() == TCP20x4Status::Ok);
  device.resetLogs();

  assert(controller.writeLine(1, "abc") == TCP20x4Status::Ok);

  assert(strcmp(controller.core().cachedLine(1), "abc                 ") == 0);
  assert(device.writes.size() == 1);
  assert(device.writes[0].line == 1);
  assert(device.writes[0].text == "abc                 ");
  assert(!controller.core().isLineDirty(1));
}

static void testWriteWhileHiddenDefersUntilDisplayOn() {
  FakeDevice device;
  TCP20x4Controller controller(device);

  assert(controller.begin() == TCP20x4Status::Ok);
  device.resetLogs();

  assert(controller.displayOff() == TCP20x4Status::Ok);
  assert(device.displayCalls.size() == 1);
  assert(device.displayCalls[0] == 0);

  device.resetLogs();

  assert(controller.writeLine(2, "hidden") == TCP20x4Status::Ok);
  assert(strcmp(controller.core().cachedLine(2), "hidden              ") == 0);
  assert(controller.core().isLineDirty(2));
  assert(device.writes.empty());

  assert(controller.displayOn() == TCP20x4Status::Ok);
  assert(device.displayCalls.size() == 1);
  assert(device.displayCalls[0] == 1);
  assert(device.writes.size() == TCP20x4Core::kRows);
  assert(device.writes[2].line == 2);
  assert(device.writes[2].text == "hidden              ");

  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(!controller.core().isLineDirty(row));
  }
}

static void testClearAndBrightnessSync() {
  FakeDevice device;
  TCP20x4Controller controller(device);

  assert(controller.begin() == TCP20x4Status::Ok);
  assert(controller.writeLine(0, "top") == TCP20x4Status::Ok);
  assert(controller.writeLine(1, "mid") == TCP20x4Status::Ok);

  device.resetLogs();

  assert(controller.clearLine(0) == TCP20x4Status::Ok);
  assert(device.writes.size() == 1);
  assert(device.writes[0].line == 0);
  assert(device.writes[0].text == "                    ");

  device.resetLogs();

  assert(controller.clear() == TCP20x4Status::Ok);
  assert(device.writes.size() == TCP20x4Core::kRows);
  for (uint8_t row = 0; row < TCP20x4Core::kRows; ++row) {
    assert(device.writes[row].text == "                    ");
  }

  device.resetLogs();

  assert(controller.setBrightness(17) == TCP20x4Status::Ok);
  assert(device.brightnessCalls.size() == 1);
  assert(device.brightnessCalls[0] == 17);
}

int main() {
  testBeginSyncsBlankDisplayAndDefaults();
  testWriteLineWritesThroughCacheWhenVisible();
  testWriteWhileHiddenDefersUntilDisplayOn();
  testClearAndBrightnessSync();
  return 0;
}
// tests/host/test_BB_tcp20x4_controller_api.cpp v1