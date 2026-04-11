#include <Arduino.h>
#include <BitBang_I2C.h>
#include <TCP20x4.h>

namespace {

constexpr uint8_t kConfiguredAddress = 0x27;
constexpr bool kConfiguredBacklightActiveHigh = true;
constexpr size_t kCommandBufferSize = 64;

// Set these to the pins you want for the LCD bit-banged I2C bus.
constexpr uint8_t kBusSdaPin = 10;
constexpr uint8_t kBusSclPin = 11;
constexpr uint32_t kBusFrequencyHz = 100000;

TCP20x4Pcf8574Config makeLcdConfig() {
  TCP20x4Pcf8574Config config =
      TCP20x4Pcf8574Config::CommonYwRobot(kConfiguredAddress);
  config.pinMap.backlightActiveHigh = kConfiguredBacklightActiveHigh;
  return config;
}

const TCP20x4Pcf8574Config kLcdConfig = makeLcdConfig();

BBI2C lcdBus{};
TCP20x4 lcd(lcdBus, kLcdConfig);
char commandBuffer[kCommandBufferSize];

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

void printHexByte(uint8_t value) {
  if (value < 0x10) {
    Serial.print('0');
  }
  Serial.print(value, HEX);
}

void setupLcdBus() {
  memset(&lcdBus, 0, sizeof(lcdBus));
  lcdBus.bWire = 0;  // use bit-banged I2C
  lcdBus.iSDA = kBusSdaPin;
  lcdBus.iSCL = kBusSclPin;
  I2CInit(&lcdBus, kBusFrequencyHz);
}

bool addressPresent(const uint8_t* map, uint8_t address) {
  return (map[address >> 3] & (1u << (address & 0x07))) != 0;
}

void printPrompt() {
  Serial.println();
  Serial.println("> ? for help");
}

void printHelp() {
  Serial.println("Commands:");
  Serial.println(" ? help");
  Serial.println(" s scan common PCF8574 ranges");
  Serial.println(" r re-run begin() and demo lines");
  Serial.println(" c clear");
  Serial.println(" o display off");
  Serial.println(" n display on");
  Serial.println(" b backlight off");
  Serial.println(" B backlight on");
  Serial.println(" 0 text write line 0");
  Serial.println(" 1 text write line 1");
  Serial.println(" 2 text write line 2");
  Serial.println(" 3 text write line 3");
}

void scanRange(const uint8_t* map, uint8_t first, uint8_t last) {
  for (uint8_t address = first; address <= last; ++address) {
    if (addressPresent(map, address)) {
      Serial.print(" found 0x");
      printHexByte(address);
      Serial.println();
    }
  }
}

void scanCommonPcf8574Addresses() {
  uint8_t map[16];
  memset(map, 0, sizeof(map));
  I2CScan(&lcdBus, map);

  Serial.println("Scanning 0x20-0x27 and 0x38-0x3F");
  scanRange(map, 0x20, 0x27);
  scanRange(map, 0x38, 0x3F);
}

void runDemo() {
  char line0[21];
  snprintf(line0, sizeof(line0), "addr 0x%02X", kLcdConfig.address);

  Serial.println("Running begin() and demo writes");
  printStatus("begin", lcd.begin());
  printStatus("writeLine(0)", lcd.writeLine(0, line0));
  printStatus("writeLine(1)", lcd.writeLine(1, "row1 display test"));
  printStatus("writeLine(2)", lcd.writeLine(2, "row2 backlight"));
  printStatus("writeLine(3)", lcd.writeLine(3, "row3 ? for help"));
}

void trimTrailingLineEndings(char* text) {
  size_t len = strlen(text);
  while (len > 0 && (text[len - 1] == '\r' || text[len - 1] == '\n')) {
    text[len - 1] = '\0';
    --len;
  }
}

void handleCommand(char* command) {
  trimTrailingLineEndings(command);

  if (command[0] == '\0') {
    printPrompt();
    return;
  }

  switch (command[0]) {
    case '?':
      printHelp();
      break;
    case 's':
      scanCommonPcf8574Addresses();
      break;
    case 'r':
      runDemo();
      break;
    case 'c':
      printStatus("clear", lcd.clear());
      break;
    case 'o':
      printStatus("displayOff", lcd.displayOff());
      break;
    case 'n':
      printStatus("displayOn", lcd.displayOn());
      break;
    case 'b':
      printStatus("backlightOff", lcd.backlightOff());
      break;
    case 'B':
      printStatus("backlightOn", lcd.backlightOn());
      break;
    case '0':
    case '1':
    case '2':
    case '3': {
      const uint8_t line = static_cast<uint8_t>(command[0] - '0');
      const char* text = command + 1;
      if (*text == ' ') {
        ++text;
      }
      printStatus("writeLine", lcd.writeLine(line, text));
      break;
    }
    default:
      Serial.println("Unknown command");
      break;
  }

  printPrompt();
}

bool readCommandLine(char* buffer, size_t bufferSize) {
  if (!Serial.available()) {
    return false;
  }

  const size_t bytesRead = Serial.readBytesUntil('\n', buffer, bufferSize - 1);
  buffer[bytesRead] = '\0';
  return true;
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  setupLcdBus();

  Serial.println();
  Serial.println("TCP20x4 HardwareSmoke");
  Serial.print("Configured address: 0x");
  printHexByte(kLcdConfig.address);
  Serial.println();
  Serial.print("Backlight polarity: ");
  Serial.println(kLcdConfig.pinMap.backlightActiveHigh ? "active high" : "active low");
  Serial.print("SDA pin: ");
  Serial.println(kBusSdaPin);
  Serial.print("SCL pin: ");
  Serial.println(kBusSclPin);

  scanCommonPcf8574Addresses();
  printHelp();
  runDemo();
  printPrompt();
}

void loop() {
  if (readCommandLine(commandBuffer, sizeof(commandBuffer))) {
    handleCommand(commandBuffer);
  }
}