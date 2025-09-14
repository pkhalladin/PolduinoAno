# Polduino Ano V1

A PlatformIO project for the Polduino Ano V1 board featuring an interactive OLED animation with user controls.

## Features

- **OLED Display Animation**: Animated rotating squares bouncing around a 128x64 OLED display
- **Interactive Controls**: Two buttons to control animation speed
- **Built-in LED Control**: Toggle LED state using button combinations
- **I2C Communication**: SSD1306 OLED display via I2C interface

## Hardware Requirements

- Polduino Ano V1 board (ATmega328P-based)
- SSD1306 OLED display (128x64, I2C address 0x3C)
- Two push buttons connected to pins D2 and D3
- Built-in LED on pin 13

## Dependencies

This project uses the following libraries (automatically managed by PlatformIO):

- `Adafruit SSD1306` v2.5.13 - OLED display driver
- `Bounce2` v2.72 - Button debouncing library
- `SSD1306Ascii` v1.3.5 - Alternative ASCII display library

## Controls

- **D2 Button**: Decrease animation speed
- **D3 Button**: Increase animation speed
- **D2 + D3 (simultaneous)**: Toggle built-in LED state

Animation speed can be adjusted between 1 (slowest) and 5 (fastest).

## Building and Uploading

### Prerequisites

1. Install [PlatformIO](https://platformio.org/install)
2. Install [Visual Studio Code](https://code.visualstudio.com/) with PlatformIO extension (recommended)

### Build and Upload

```bash
# Build the project
pio run

# Upload to board (via USB)
pio run --target upload
```

## Bootloader Installation

If you need to install or reinstall the bootloader on your ATmega328P microcontroller, use the following command with an AVR programmer (e.g., USBasp):

```bash
avrdude -c usbasp -p m328p -U flash:w:optiboot_atmega328.hex:i -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m
```

### Bootloader Command Breakdown

- `-c usbasp`: Use USBasp programmer
- `-p m328p`: Target ATmega328P microcontroller
- `-U flash:w:optiboot_atmega328.hex:i`: Write bootloader to flash memory
- `-U lfuse:w:0xFF:m`: Set low fuse byte to 0xFF (16MHz crystal, fast startup)
- `-U hfuse:w:0xDE:m`: Set high fuse byte to 0xDE (bootloader enabled, 512 words bootloader size)
- `-U efuse:w:0x05:m`: Set extended fuse byte to 0x05 (brown-out detection at 2.7V)

**Note**: Make sure you have the correct `optiboot_atmega328.hex` file in your project root directory before running the bootloader installation command.

## Project Structure

```
PolduinoAno/
├── src/
│   └── main.cpp           # Main application code
├── include/               # Header files
├── lib/                   # Local libraries
├── test/                  # Unit tests
├── platformio.ini         # PlatformIO configuration
├── optiboot_atmega328.hex # Bootloader binary
└── README.md             # This file
```

## Animation Details

The project displays an animated scene with 7 rotating square pixels that bounce around the screen. Each pixel:

- Moves with normalized velocity vectors
- Rotates continuously based on its movement
- Bounces off screen edges
- Has randomized initial position, velocity, and rotation

## Troubleshooting

### Display Issues
- Ensure SSD1306 display is connected to the correct I2C pins
- Check I2C address (default: 0x3C)
- Verify power connections (3.3V/5V compatibility)

### Programming Issues
- Make sure the board is properly connected via USB
- Check that the correct board type is selected in `platformio.ini`
- If bootloader is corrupted, use the bootloader installation command above

### Button Issues
- Verify buttons are connected to pins D2 and D3
- Check pull-up resistor configuration
- Adjust debounce interval if needed (currently set to 5ms)

## License

This project is open source. Please check with the original author for specific licensing terms.

## Author

Created for the Polduino Ano V1 development board.