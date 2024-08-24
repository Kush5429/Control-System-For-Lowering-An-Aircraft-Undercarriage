# Aircraft Undercarriage Control System #

This repository contains a control system for managing the lowering of an aircraft's landing gear using an 8051 microcontroller. The system is designed to meet precise timing requirements for output generation and response monitoring, utilizing timer interrupts for efficient task management and power savings.

## Overview

The control system handles the lowering of the aircraft's undercarriage by generating a sequence of output signals. The process is initiated by pressing the "Start" button and can be aborted by pressing the "Stop" button. The system uses Timer 0 interrupts to manage timing with high precision and enters idle mode to save power when not actively processing tasks.

## Features

- **Precise Timing Control**: Utilizes Timer 0 for accurate timing of output signals.
- **Power Efficiency**: Implements idle mode to reduce power consumption when idle.
- **Fail-Safe Operation**: Includes a mechanism to abort operations within 10ms if the stop button is pressed.

## Timing Requirements

The system operates according to the following sequence:

| Output Pin | Output Time | Response Time | Response Pin |
|-------------|-------------|---------------|--------------|
| P1.0        | 10 seconds   | 1 second      | P2.0         |
| P1.2        | 17 seconds   | 2 seconds      | P2.1         |
| P2.7        | 30 seconds   | 1 second      | P2.2         |
| P3.1        | 32 seconds   | 3 seconds      | P2.3         |
| P2.6        | 40 seconds   | 1 second      | P2.4         |

## Code

The provided code implements the following functionality:

- **Initialization**: Configures Timer 0 to generate interrupts at 1ms intervals and sets up idle mode for power saving.
- **Main Loop**: Monitors the state of start and stop buttons, manages system operation, and enters idle mode when inactive.
- **Timer Interrupt**: Manages the timing and execution of output steps, ensuring tasks are handled with precise timing.
- **Output Control**: Activates and deactivates output pins according to the specified timing requirements.
- **Stop Button Check**: Monitors the stop button and aborts the operation if pressed.
