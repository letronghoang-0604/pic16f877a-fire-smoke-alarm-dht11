# pic16f877a-fire-smoke-alarm-dht11
Fire and smoke alarm using PIC16F877A, DHT11 and MQ-2 (CCS C)

# Fire and Smoke Alarm using PIC16F877A (CCS C)

This is a small embedded project using a **PIC16F877A** microcontroller and **CCS C** compiler to monitor temperature, humidity and gas/smoke level for fire/smoke warning.  
The system reads temperature and humidity from a **DHT11 sensor**, checks for gas/smoke using an **MQ-2 gas sensor**, displays values and status on a **16x2 LCD**, and activates a **buzzer** when dangerous conditions are detected.

## Features

- Measure ambient **temperature (°C)** and **humidity (%)** using a **DHT11** sensor.
- Display live temperature and humidity on a **16x2 character LCD**.
- Detect gas/smoke using an **MQ-2** gas sensor (digital output).
- Combine **over-temperature** and **gas** conditions:
  - Normal: temperature below threshold and no gas.
  - Over-temperature only.
  - Gas only.
  - Both over-temperature and gas/smoke present.
- Use a **buzzer** for audible alarm in any abnormal condition.
- Show clear status messages on the LCD (normal / over-temperature / gas / both).

## Hardware

- Microcontroller: **PIC16F877A** (20 MHz external crystal)
- Temperature & humidity sensor: **DHT11**
- Gas/smoke sensor: **MQ-2** gas sensor module (digital output)
- 16x2 character LCD (4-bit mode)
- Buzzer (connected to a GPIO pin, optionally via driver transistor)
- Power supply (5 V for logic and sensors)
- Miscellaneous: resistors, wires, breadboard or PCB

## Software & Tools

- **Compiler**: CCS C for PIC (`#include <16F877A.h>`)  
- Clock: 20 MHz (`#use delay(clock = 20000000)`)
- LCD library: `lcd.c` (standard CCS LCD driver)
- IDE: CCS IDE or MPLAB X with CCS plug-in
- Programmer: PICkit or compatible programmer for PIC16F877A
- Optional: Proteus for circuit simulation

## Code Overview

Main source file: `fire_smoke_alarm.c` (original name: `lam.c`, placed under the `code/` folder).

Key points:

- A custom DHT11 read function bit-bangs the protocol on the DHT11 data pin:
  - Sends the start signal, waits for the sensor’s response.
  - Reads 5 bytes (humidity integer/decimal, temperature integer/decimal, checksum).
  - Verifies the checksum; on success, stores:
    - temperature (°C)
    - humidity (%)
- Gas detection uses the digital output of the MQ-2 sensor:
  - Logic high/low indicates whether gas/smoke level crosses the threshold.
- Over-temperature condition is detected by comparing the measured temperature against a threshold (e.g. 40 °C).
- LCD operation:
  - First line shows live readings: temperature and humidity.
  - Second line shows status messages:
    - Both high temperature and gas.
    - High temperature only.
    - Gas only.
    - Normal.
- The buzzer is turned ON whenever there is over-temperature and/or gas, and turned OFF only in the normal state.

## How It Works (High-Level)

1. **Initialize:**
   - Configure I/O directions for DHT11, MQ-2, LCD and buzzer pins.
   - Initialize the LCD and clear the display.
   - Optionally show an intro message (project title, author, student ID).

2. **In the main loop:**
   - Read temperature and humidity from the DHT11 sensor.
   - Update the LCD with the current temperature and humidity values.
   - Read the MQ-2 digital output to detect gas/smoke.
   - Evaluate the conditions:
     - Over-temperature only.
     - Gas only.
     - Both over-temperature and gas.
     - Normal (no alarms).
   - According to the current condition:
     - Display the corresponding status message on the LCD.
     - Turn the buzzer ON for any abnormal condition, OFF in normal state.
   - Wait a short delay and repeat the loop to continuously monitor the environment.
