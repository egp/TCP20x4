# TCP20x4

`TCP20x4` is a lean Arduino library for 20x4 HD44780-compatible character LCDs driven through a PCF8574 I2C backpack.

## Status

The library is now at a usable v1 state:

- host tests are green
- GitHub Actions is green
- Arduino example compilation is green
- `HardwareSmoke` has been validated on Arduino UNO R3 and UNO R4 WiFi with a working 20x4 backpacked display

## Design goals

- Keep the public API small and line-oriented
- Keep hardware-agnostic logic host-testable
- Keep the Arduino/Wire/PCF8574 layer thin
- Treat backpack wiring details as configuration, not as universal constants
- Provide a practical bring-up sketch for real hardware

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
   - concrete Arduino/Wire-backed device for HD44780 over PCF8574
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

## Non-goals for v1

Out of scope for this release:

- scrolling features
- font management
- general custom glyph facilities
- brightness/PWM control
- software “180 degree rotation”

Note: physical upside-down mounting of an HD44780 character LCD is not corrected in software. Mechanical orientation must be handled physically.

## Example

See:

- `examples/HardwareSmoke/HardwareSmoke.ino`

This sketch is intended for first-board bring-up and diagnostics. It supports:

- I2C scanning of common PCF8574 address ranges
- `begin()` and demo writes
- display on/off
- backlight on/off
- direct line writes through the serial monitor

## Development/testing

Host-side tests cover:

- core initial state
- line validation and padding
- clear and clearLine behavior
- display-off/display-on cache semantics
- controller black-box behavior
- PCF8574 transport helper behavior

CI runs:

- host tests
- Arduino example compile

## Repository

- Remote: `https://github.com/egp/TCP20x4`