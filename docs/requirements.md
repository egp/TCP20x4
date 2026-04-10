# TCP20x4 Requirements

## 1. Scope and intent

### 1.1
The project shall provide a lean Arduino library for a 20x4 HD44780-compatible
character LCD accessed through a PCF8574-based I2C backpack.

### 1.2
The library shall prioritize:
- a small public API
- deterministic behavior
- host-testable logic
- simple hardware bring-up
- incremental test-driven development

### 1.3
The library shall exclude non-essential device features from v1 unless they are
required to support the primary line-oriented display use case.

## 2. Architecture

### 2.1
The hardware-facing layer shall be as thin as practical.

### 2.2
The thin hardware layer shall be limited to:
- I2C transport through `Wire`
- PCF8574 byte writes
- LCD timing delays required by the protocol
- LCD command/data emission
- minimal Arduino-facing glue

### 2.3
All logic that does not inherently require Arduino headers or hardware access
shall be isolated in host-testable C++.

### 2.4
Host-testable logic shall include at minimum:
- row-to-DDRAM address mapping
- line-length validation
- right-padding shorter lines with spaces
- cache maintenance
- display-on redraw behavior
- nibble-to-port-bit encoding
- enable pulse sequencing
- backlight/brightness planning behavior where applicable

### 2.5
Backpack-to-LCD pin mapping shall be treated as configuration, not as a fixed
universal standard.

## 3. Character handling

### 3.1
The library shall not implement font management.

### 3.2
The library shall rely on the LCD controller's built-in character generator for
ordinary character display.

### 3.3
The minimum required supported character subset shall include:
- `A-Z`
- `a-z`
- `0-9`
- space
- percent sign `%`
- decimal point `.`
- any additional punctuation needed by tests and diagnostics

### 3.4
Supporting the full printable 7-bit ASCII range is acceptable and preferred if
it is straightforward.

### 3.5
Custom glyph definition support shall not be required for v1.

## 4. Public API behavior

### 4.1
The primary client-facing write operation shall be line-oriented.

### 4.2
The library shall provide an operation equivalent to:
- `writeLine(line, text)`

### 4.3
The line index domain shall be exactly:
- `0`
- `1`
- `2`
- `3`

### 4.4
A line write shorter than 20 characters shall be padded on the right with
spaces to a full 20-character logical line.

### 4.5
A line write longer than 20 characters shall return an error.

### 4.6
A line index outside `0..3` shall return an error.

### 4.7
An overlength line write shall not partially update the target line.

### 4.8
The library shall provide display visibility control equivalent to:
- `displayOn()`
- `displayOff()`

### 4.9
The library shall provide clear operations equivalent to:
- `clear()`
- `clearLine(line)`

### 4.10
The library may expose additional low-level or convenience APIs, but the
line-oriented API shall remain the primary supported interface.

## 5. Cache and redraw semantics

### 5.1
The library shall maintain a full cached 4x20 logical display image.

### 5.2
The cache shall be the source of truth for intended display contents.

### 5.3
Writing a line while the display is on shall update both:
- the cache
- the visible display

### 5.4
Writing a line while the display is off shall update the cache.

### 5.5
Turning the display off shall not discard cached content.

### 5.6
Turning the display back on shall cause the visible display to match the most
recent cached content.

### 5.7
The implementation may satisfy 5.6 by forcing a redraw from cache.

## 6. Device features in scope and out of scope

### 6.1
The library shall support ordinary text display on all four visible lines.

### 6.2
The library shall support display on/off.

### 6.3
If the hardware variant supports adjustable brightness or backlight intensity,
the library shall expose brightness control in the API.

### 6.4
If the hardware variant supports only binary backlight control, the API shall
support at least backlight on/off semantics.

### 6.5
Scrolling features shall not be required for v1.

### 6.6
Other advanced LCD features shall remain out of scope for v1 unless needed to
support the required line-oriented behavior.

## 7. Error handling

### 7.1
The library shall return explicit status or error information for:
- invalid line index
- line length greater than 20
- initialization failure
- transport failure
- unsupported requested brightness behavior on the current hardware variant

### 7.2
The library shall prefer explicit error returns over silent truncation.

### 7.3
The library shall avoid partial updates on invalid client requests.

## 8. Testing and verification

### 8.1
The project shall include substantial host-side white-box tests.

### 8.2
Host-side tests shall run on macOS and in GitHub Actions.

### 8.3
Host-side tests shall not depend on Arduino headers or physical hardware.

### 8.4
The project shall include black-box tests for public API behavior where
possible on host side.

### 8.5
GitHub Actions shall run host-side tests on each push and pull request.

### 8.6
GitHub Actions shall compile the operator-feedback sketch.

### 8.7
The test plan shall intentionally support red/green cycles by allowing:
- stub implementations that compile
- targeted WB tests for internal helpers
- targeted BB tests for public API behavior

## 9. Hardware-in-the-loop verification

### 9.1
The project shall include an operator-feedback sketch named:
- `examples/HardwareSmoke/HardwareSmoke.ino`

### 9.2
The hardware smoke sketch shall support manual bring-up and validation of:
- I2C connectivity
- backpack detection or configured addressing
- LCD initialization
- display on/off behavior
- backlight behavior
- line addressing on all four rows
- visible text output
- any supported brightness behavior

### 9.3
The hardware smoke sketch shall be suitable for iterative extension during
development.

## 10. Development process

### 10.1
The implementation shall proceed API by API in small test-driven increments.

### 10.2
For each API or dependency slice, development shall follow this pattern:
- add a WB or BB test first
- prove the new test fails
- replace the relevant stub with production code
- iterate until the test is green
- commit the green state

### 10.3
Bottom-up implementation of dependencies is acceptable and expected where an API
depends on lower-level behavior.

## 11. Initial API set for stubbing

### 11.1
The initial stubbed codebase shall define the public APIs needed to support:
- initialization
- line write
- clear
- clear line
- display on
- display off
- backlight on/off andor brightness control as applicable
- any minimal status/error reporting types needed by the above

### 11.2
The initial stubbed codebase shall compile before any API is fully implemented.