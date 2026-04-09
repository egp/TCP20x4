# TCP20x4 Project Plan

## Objective

Create a small, testable Arduino library for 20x4 HD44780-compatible character
LCDs attached through a PCF8574 I2C backpack.

## Design principles

1. Keep the hardware layer extremely thin:
   - `Wire` calls
   - PCF8574 byte writes
   - timing delays

2. Push logic into host-testable code:
   - row-to-DDRAM mapping
   - port-bit encoding
   - backlight polarity handling
   - nibble pulse sequencing

3. Treat backpack pin mapping as configuration, not a baked-in assumption.

4. Use hardware smoke tests for electrical and wiring validation.

5. Use GitHub CI for host-testable behavior and sketch compile checks.

## Phases

### Phase 0 — Scaffold

- Create repo structure
- Add license, README, gitignore, CI
- Add docs
- Add first `HardwareSmoke.ino`

### Phase 1 — Minimal usable library

- Implement PCF8574 port write
- Implement HD44780 4-bit initialization
- Implement command/data byte send
- Implement:
  - `begin()`
  - `clear()`
  - `home()`
  - `setCursor()`
  - `write()`
  - `backlightOn()`
  - `backlightOff()`

### Phase 2 — Expand host coverage

- Add host tests for:
  - all row offsets
  - multiple pin maps
  - backlight active-high and active-low
  - pulse sequences
  - command encoding helpers

### Phase 3 — Better ergonomics

- Add optional constructors for common backpack layouts
- Add display/cursor/blink controls
- Add `printAt(col,row,...)` convenience
- Add optional I2C address scan helper for smoke testing

### Phase 4 — Hardening

- Improve error reporting and return values
- Validate edge conditions
- Document known backpack mapping variants
- Add additional example sketches

## Non-goals for early versions

- automatic backpack pin-map detection
- busy-flag reads through RW
- CGRAM custom glyph helpers
- animation helpers
- board-specific hacks

## Acceptance bar for code-complete v1

- library compiles in CI
- host tests cover core mapping logic
- smoke sketch proves:
  - I2C connectivity
  - backlight polarity
  - nibble ordering
  - row addressing
  - basic text output