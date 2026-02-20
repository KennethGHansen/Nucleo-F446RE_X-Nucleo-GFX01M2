
# NUCLEO-F446RE + X-NUCLEO-GFX01M2 Display Driver (ST7789)

This repository contains a working example project demonstrating how to drive the  
**DT022CTFT (ST7789) TFT display module** using an **ST NUCLEO‑F446RE** and the  
**X‑NUCLEO‑GFX01M2** display expansion board. The NUCLEO-F446RE contains the STM32F446RE microcontroller

It includes a fully working low-level driver (`st7789h2.c/h`) with:

- Correct initialization sequence for the DT022CTFT panel  
- Correct visible window offset inside the 240×320 ST7789 RAM  
- Working rotation and BGR color mode  
- Pixel drawing, full‑screen fill, and scaled ASCII text rendering  
- A simple 5×7 ASCII font, including scaling and `\n` line breaks  

This project is written for **Keil uVision** but is compatible with STM32CubeIDE as well.

---

## Hardware

### Required boards
- **NUCLEO‑F446RE** (main MCU board)
- **X‑NUCLEO‑GFX01M2** (display expansion adapter)
- **DT022CTFT‑D01** display module (ST7789 controller)

The X‑NUCLEO‑GFX01M2 expansion board plugs into the NUCLEO-F446RE board via the MORPHO connectors. But my experience was
that when doing so, I could not get the debug interface working. So I did the wiring with jumper wires
instead. The wiring is done on basis of the port setup i did in CubeMX:

<img width="622" height="616" alt="image" src="https://github.com/user-attachments/assets/26855a65-70c8-4925-b4c8-d13d40b08085" />

| NUCLEO-F446RE | X-NUCLEO-GFX01M2 | FUNCTION |
|---------------|------------------|----------|

| PA1(GPIO_OUT) - CN7_30 | CN2_30 | Display reset |


PB5(GPIO_OUT)   - CN10_29        CN3_25                    Display SPI write enable
PA4(GPIO_OUT)   - CN7_32         CN3_21                    Display SPI chip select active high
PA5(SPI1_CLK)   - CN10_11        CN3_11                    Display SPI serial clock
PA6(SPI1_MISO)  - CN10_13        CN3_13                    Display SPI master in/slave out (Optional)
PA7(SPI1_MOSI)  - CN10_15        CN3_15                    Display SPI master out/slave in
VDD(3V3)        - CN7_5          CN2_16                    X-NUCLEO-GFX01M2 board supply
GND             - CN7_8          CN2_4                     GROUND

| Function | Signal | MCU Pin | Notes |
|----------|--------|----------|-------|
| LCD Reset | DISP_NRESET | PA8 | Active‑low |
| Data/Command | DISP_DCX | PB5 | GPIO |
| Chip Select | DISP_NCS | PA4 | GPIO |
| SPI Clock | DISP_SCL | PA5 | SPI1_SCK |
| SPI MOSI | DISP_SDA | PA7 | SPI1_MOSI |
| SPI MISO | DISP_SDO | PA6 | Optional |

GPIO setup:
PA1 - PB5 - HIGH (PP)
PA4 - LOW (PP)

SPI setup:
Full Duplex Master - Motorola, 8 bit, MSB first, 45 MBps, CPOL-Low, CPHA-1 Edge

---

## Features

- Full ST7789 register-level initialization (no external libraries needed)
- 240×320 active drawing area
- MADCTL for upright, non‑mirrored orientation
- RGB565 color support (BGR internally for ST7789)
- Pixel drawing (`DrawPixel`)
- Full‑screen fill (`Fill`)
- ASCII text support:
  - 5×7 classic font
  - Scaled text (1×, 2×, 3×…)
  - Line breaks using `\n`
- Ready for additional graphics functions (lines, rectangles, images)

---

## Project Structure
X-Nucleo-GFX01M2/
├── Core/
│   ├── Inc/
│   └── Src/
│
├── Drivers/
│   └── Custom/
│       ├── st7789h2.c
│       ├── st7789h2.h
│       ├── font5x7.c
│       └── font5x7.h
│
├── MDK-ARM/
│   ├── MyProject.uvprojx
│   ├── MyProject.uvoptx
│
├── README.md
├── LICENSE
├── .gitignore
└──X-Nucleo-GFX01M2.ioc

Keil users: be sure to add  
`Drivers/Custom/ST7789/`  
to the **include paths in uvision**

---

##  Getting Started

### 1. Clone the repository

### 2. Open the Keil `.uvprojx` project file

### 3. Make sure the include paths are correct
Keil →  
**Project → Options for Target → C/C++ → Include Paths**
Add:
../Drivers/Custom

### 4. Build and flash
Press **Build** and **Download** in Keil.

The display should:

1. Initialize properly  
2. Fill the screen  
3. Display scaled ASCII text


## Example Code

main.c (outcrop - see main.c):

ST7789H2_Init();
ST7789H2_Fill(0x0000); //Black background

ST7789_DrawStringScaled(10, 10, "Hello\nWorld!", 
                        0xFFFF, 0x0000, 
                        2);
//Write ASCII strings (x, y, string, color, letter background color, letter scaling)
ST7789H2_DrawStringScaled(50, 50, "Hello World!\n\nJust a Test\n\nTo see\n\nIf it works", 0xFFFF, 0x0000,2);

---

## License
This project is released under the MIT License.
You are free to use, modify, and share.

---

## Screenshots

![20260220_113315](https://github.com/user-attachments/assets/abbed471-4590-45cb-9af8-72f2f9a59fb1)
![20260220_113305](https://github.com/user-attachments/assets/4cebaeaf-5728-4322-a16e-ee42099f6ac6)
![20260220_113258](https://github.com/user-attachments/assets/039a14a4-ceb8-4f11-9eaf-5f8d787a57a7)


---

## Acknowledgements
Thanks to the ST community, the X‑NUCLEO‑GFX01M2 hardware documentation,
and the open‑source embedded graphics community

---






