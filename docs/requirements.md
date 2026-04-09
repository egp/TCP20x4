# TCP20x4 Requirements

## 1. Repository and tooling

### 1.1
The project shall be structured as an Arduino library repository.

### 1.2
The repository shall include:
- `LICENSE`
- `README.md`
- `.gitignore`
- `.github/workflows/ci.yml`
- `docs/project_plan.md`
- `docs/requirements.md`
- `examples/HardwareSmoke/HardwareSmoke.ino`

## 2. Architecture

### 2.1
The hardware-facing layer shall be as thin as practical.

### 2.2
The thin hardware layer shall be limited to:
- I2C transport through `Wire`
- PCF8574 byte writes
- timing delays required by the LCD protocol
- thin public Arduino-facing API glue

### 2.3
All logic that does not inherently require Arduino hardware APIs shall be
isolated in host-testable C++.

### 2.4
Host-testable logic shall include at minimum:
- row-to-DDRAM address mapping
- nibble-to-port-bit encoding
- enable pulse sequencing
- backlight polarity handling

## 3. Functional behavior

### 3.1
The library shall support a 20x4 HD44780-compatible LCD connected through a
PCF8574-based backpack.

### 3.2
The library shall support explicit runtime configuration of backpack pin
mapping.

### 3.3
The library shall support at minimum:
- `begin()`
- `clear()`
- `home()`
- `setCursor(col,row)`
- single-character write
- printable text through Arduino `Print`
- `backlightOn()`
- `backlightOff()`

### 3.4
The library shall initialize the LCD in 4-bit mode.

### 3.5
The library shall support 20x4 row offsets:
- row 0 => `0x00`
- row 1 => `0x40`
- row 2 => `0x14`
- row 3 => `0x54`

## 4. Testing

### 4.1
The project shall include host-side tests runnable on macOS and GitHub Actions.

### 4.2
The host-side tests shall not depend on Arduino headers or Arduino hardware.

### 4.3
GitHub Actions shall run host-side tests on each push and pull request.

### 4.4
GitHub Actions shall compile the `HardwareSmoke` example sketch.

## 5. Hardware smoke verification

### 5.1
A smoke sketch shall exist for first-board validation.

### 5.2
The smoke sketch shall visibly verify:
- backlight control
- display clear/home
- line addressing on all four rows
- ordinary character output

### 5.3
The smoke sketch shall be suitable for manual extension as the primary early
hardware validation tool.