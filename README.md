# TCP20x4

TCP20x4 is a lean Arduino library for 20x4 HD44780-compatible character LCDs driven through a PCF8574 I2C backpack.

The library is built around a small public API, a host-testable cache/core, and a thin backpack transport layer. The Arduino-facing transport dependency is `TCP1819`, which supplies the `BBI2C` bus type used by the public constructors.

## Status

The library is at a practical v1 state:

- host tests are green
- GitHub Actions is green
- Arduino example compilation is part of CI
- `HardwareSmoke` has been used for real-hardware bring-up on common backpacked 20x4 displays

## Companion dependency

TCP20x4 depends on `TCP1819` for the Arduino-facing I2C transport type and helpers.

The public constructors take a `BBI2C&`:

- `TCP20x4(BBI2C& i2c, const TCP20x4Pcf8574Config& config)`
- `TCP20x4(BBI2C& i2c, uint8_t i2cAddress, const TCP20x4PinMap& pinMap)`

Install both libraries before building examples:

- `TCP20x4`
- `TCP1819`

Example layout:

- `~/Documents/Arduino/libraries/TCP20x4`
- `~/Documents/Arduino/libraries/TCP1819`

## Design goals

- keep the public API small and line-oriented
- keep hardware-agnostic logic host-testable
- keep the TCP1819/PCF8574 transport layer thin
- treat backpack wiring details as configuration, not universal constants
- provide a practical bring-up sketch for real hardware

## Architecture

The implementation is split into three layers:

1. `TCP20x4Core`
   - pure host-testable cache/state logic
   - validates line writes
   - pads shorter lines to 20 columns
   - preserves cached content while the display is off
   - forces redraw from cache when display is turned back on

2. `TCP20x4Controller`
   - orchestrates cache-first behavior
   - updates the core first, then syncs hardware through a device interface

3. `TCP20x4Pcf8574Device`
   - concrete TCP1819-backed device for HD44780 over PCF8574
   - handles I2C writes, LCD initialization, command/data emission, backlight on/off, and full-line writes

## Public API

Primary operations:

- `begin()`
- `writeLine(line, text)`
- `clear()`
- `clearLine(line)`
- `displayOn()`
- `displayOff()`
- `backlightOn()`
- `backlightOff()`

Behavior:

- valid lines are `0..3`
- lines longer than 20 characters return an error
- shorter lines are padded on the right with spaces
- the cache is the source of truth
- writes while display is off update cache only
- `displayOn()` redraws from cache

## Hardware configuration

The backpack is configured explicitly rather than auto-detected.

`TCP20x4Pcf8574Config` includes:

- I2C address
- backpack pin map
- backlight polarity
- row offsets

Provided convenience preset:

- `TCP20x4Pcf8574Config::CommonYwRobot(address)`

The common default row offsets for a 20x4 display are:

- `0x00`
- `0x40`
- `0x14`
- `0x54`

## Minimal example

#include <string.h>
#include <TCP1819.h>
#include <TCP20x4.h>

namespace {
constexpr uint8_t kLcdAddress = 0x27;
constexpr uint8_t kBusSdaPin = 9;
constexpr uint8_t kBusSclPin = 10;
constexpr uint32_t kBusFrequencyHz = 100000UL;

BBI2C lcdBus{};
TCP20x4Pcf8574Config lcdConfig =
    TCP20x4Pcf8574Config::CommonYwRobot(kLcdAddress);
TCP20x4 lcd(lcdBus, lcdConfig);

void setupLcdBus() {
  memset(&lcdBus, 0, sizeof(lcdBus));
  lcdBus.bWire = 0;
  lcdBus.iSDA = kBusSdaPin;
  lcdBus.iSCL = kBusSclPin;
  I2CInit(&lcdBus, kBusFrequencyHz);
}
}  // namespace

void setup() {
  setupLcdBus();

  (void)lcd.begin();
  (void)lcd.writeLine(0, "TCP20x4 ready");
  (void)lcd.writeLine(1, "Hello");
  (void)lcd.writeLine(2, "20x4 display");
  (void)lcd.writeLine(3, "over PCF8574");
}

void loop() {
}

## Example

See:

- `examples/HardwareSmoke/HardwareSmoke.ino`

This sketch is intended for first-board bring-up and diagnostics. It supports:

- scanning common PCF8574 address ranges
- `begin()` and demo writes
- display on/off
- backlight on/off
- direct line writes through the serial monitor

The sketch keeps the backpack address, backlight polarity, and TCP1819 bus pins near the top of the file so they can be adjusted for your hardware.

## Non-goals for v1

Out of scope for this release:

- scrolling features
- font management
- general custom glyph facilities
- brightness/PWM control
- software 180-degree rotation

Note: physical upside-down mounting of an HD44780 character LCD is not corrected in software. Mechanical orientation must be handled physically.

## Development and testing

Host-side tests cover:

- core initial state
- line validation and padding
- clear and clearLine behavior
- display-off/display-on cache semantics
- controller black-box behavior
- PCF8574 transport helper behavior

CI runs:

- host tests
- Arduino example compilation

## Local development

Typical commands:

make clean test

If you compile sketches directly with Arduino CLI, install both `TCP20x4` and `TCP1819`, or pass both libraries explicitly with `--library`.

## Repository

- Remote: `https://github.com/egp/TCP20x4`

## License

MIT License.