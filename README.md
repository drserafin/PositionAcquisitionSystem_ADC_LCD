# PositionAcquisitionSystem_ADC_LCD

## Lab Description

The goal of this lab is to design and implement a real-time position measurement system using a microcontroller, a slide potentiometer, and a 16x2 LCD display. The lab is divided into two main phases.

---

## Phase 1 – LCD Driver Enhancement

- Interface a 16x2 character LCD (Hitachi HD44780) to the STM32F0 microcontroller.
- Enhance your previous LCD driver from Lab 1 to add functions that display:
  - Unsigned decimal numbers (`LCD_OutUDec`)
  - Fixed-point numbers with 0.001 resolution (`LCD_OutUFix`)
- Learn modular device driver design and debugging.

---

## Phase 2 – Position Acquisition System

- Interface a slide potentiometer with the STM32F0 ADC to measure linear position (0–2 cm).
- Write an ADC driver to sample analog voltages and calibrate them.
- Convert ADC samples to 32-bit fixed-point numbers representing position in centimeters.
- Use timer interrupts to perform periodic ADC sampling at 10 Hz (real-time operation).
- Display the measured position on the LCD continuously.
- Learn concepts of real-time data acquisition, interrupt-driven programming, fixed-point arithmetic, and mailbox-based synchronization.

---

## Key Concepts Covered

- Analog-to-digital conversion (ADC)
- Fixed-point number representation
- Real-time, interrupt-driven sampling
- Modular software design with separate driver files
- Calibration and resolution/accuracy of sensors

---

## Hardware Required

- STM32F0 microcontroller
- 16x2 LCD (HD44780)
- Slide potentiometer (10 kΩ linear)
- Optional: breadboard, jumper wires, power supply, and heartbeat LED
