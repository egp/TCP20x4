# TCP20x4

`TCP20x4` is an Arduino library for 20x4 HD44780-compatible character LCDs
driven through a PCF8574 I2C backpack.

## Goals

- Keep the Arduino/Wire/PCF8574 layer as thin as possible
- Keep LCD addressing and bit-mapping logic host-testable on macOS and CI
- Provide a hardware smoke sketch for first-board bring-up
- Start with explicit backpack pin mapping rather than fragile auto-detection

## Planned structure

- `src/` — thin Arduino-facing API and PCF8574 transport
- `test/` or host-test directory — pure C++ helpers and host-side tests
- `examples/HardwareSmoke` — manual hardware verification sketch
- `docs/project_plan.md` — phased implementation plan
- `docs/requirements.md` — initial requirements baseline

## Initial assumptions

The target hardware model is:

- PCF8574 or PCF8574A I2C I/O expander
- HD44780-compatible 20x4 LCD controller
- 4-bit LCD interface via backpack wiring

The backpack-to-LCD pin mapping is treated as configuration, not as a fixed
universal standard.

## Development strategy

The implementation should be split into two layers:

1. A very thin hardware layer that performs:
   - `Wire` setup
   - PCF8574 byte writes
   - LCD timing delays
   - minimal API glue

2. A host-testable core that performs:
   - row-to-DDRAM address mapping
   - nibble/bit encoding
   - enable pulse sequencing
   - backlight polarity handling
   - any other logic not requiring Arduino headers or hardware access

## Early roadmap

Phase 0:
- repository scaffold
- documentation
- CI
- smoke sketch

Phase 1:
- explicit pin-map configuration
- 4-bit LCD initialization
- clear/home/setCursor/write
- backlight control

Phase 2:
- host-side tests for row offsets, pulse sequences, and pin maps
- optional convenience constructors for common backpack layouts

## Hardware smoke sketch

`examples/HardwareSmoke/HardwareSmoke.ino` is the first manual validation tool.
Its early purpose is to verify:

- I2C connectivity
- display initialization
- backlight control
- row addressing
- basic text output

## Repository

Remote:
`https://github.com/egp/TCP20x4`