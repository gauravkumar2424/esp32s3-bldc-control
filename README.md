ESP32-S3 Bare-Metal BLDC Motor Control
A robust bare-metal motor control system for the ESP32-S3 microcontroller, featuring PID-based speed regulation, 20kHz PWM output, and quadrature encoder support. Designed for both real hardware (GPIO19/21) and simulation mode, this project is ideal for embedded systems development, showcasing skills in low-level programming, control systems, and ESP-IDF integration.
Project Overview
This project implements a bare-metal motor control solution for the ESP32-S3, targeting BLDC motors with precise speed regulation. It leverages a PID controller, a state machine for robust operation, and a modular design supporting both real quadrature encoder input and a simulation mode for hardware-free testing. The codebase is optimized for performance and clarity, making it a valuable resource for learning embedded systems and motor control.
Key Features

20kHz PWM Output: Generates precise PWM signals on GPIO18 for motor control.
PID Controller: Implements proportional-integral-derivative control for accurate speed regulation (default setpoint: 800 Hz).
Quadrature Encoder Support: Reads real encoder signals on GPIO19 (A) and GPIO21 (B) for precise speed measurement.
Simulation Mode: Enables testing without hardware by simulating encoder signals (SIMULATE_ENCODER macro).
State Machine: Manages motor states (INIT, STOP, RUN, FAULT) with automatic transitions and fault detection (>2000 Hz or <0 Hz).
CPU Optimization: Configures the ESP32-S3 to run at 240MHz for high-performance operation.
Comprehensive Logging: Utilizes ESP-IDF logging for real-time debugging and monitoring.

Technical Skills Demonstrated

Bare-Metal Programming: Direct hardware control without an RTOS, showcasing low-level C programming.
ESP-IDF Framework: Proficient use of ESP-IDF for GPIO, PWM, and timer management.
Control Systems: Implementation of a PID controller for closed-loop speed regulation.
Modular Design: Structured codebase with separate modules for PWM, encoder, PID, and state machine.
Simulation Techniques: Hardware-free testing with simulated encoder signals, enhancing portability.
Documentation: Clear and professional documentation for accessibility and collaboration.

Requirements

Hardware:
ESP32-S3 development board (e.g., ESP32-S3-DevKitC-1)
GPIO18 for PWM output
GPIO19 and GPIO21 for quadrature encoder input (optional for simulation mode)
Optional: Quadrature encoder, LED, or multimeter for hardware testing


Software:
ESP-IDF v5.x (tested with v5.5)
No external hardware required for simulation mode



Installation and Setup

Clone the Repository:git clone https://github.com/gauravkumar2424/esp32s3-bldc-control.git
cd esp32s3-bldc-control


Set Up ESP-IDF:source /path/to/esp-idf/export.sh


Build the Project:idf.py build


Flash and Monitor:idf.py -p /dev/ttyUSB0 flash monitor



Expected Output


<img width="1049" height="278" alt="image" src="https://github.com/user-attachments/assets/736445a4-bf4b-4eeb-ae81-0bc02f864a4a" />

...

In real encoder mode (disable SIMULATE_ENCODER), logs show:
I (xxx) ENCODER: Initialized real encoder on GPIO 19 (A), 21 (B)
I (xxx) STATE_MACHINE: Transitioned to RUN
I (xxx) ENCODER: Real speed: xxx.xx Hz, pulses: xx, time_diff: xx us
I (xxx) STATE_MACHINE: Running: Speed=xxx.xx Hz, Duty=xx.xx%

Usage

Initialization: Configures CPU clock (240MHz), PWM, and encoder (real or simulated).
PID Control: Regulates motor speed to a setpoint (default: 800 Hz) using a PID controller.
State Machine: Automatically transitions from INIT to RUN, with fault detection for out-of-range speeds.
Simulation Mode: Cycles speed from 400–1000 Hz for testing without hardware.
Real Encoder: Connect encoder A and B signals to GPIO19 and GPIO21 for actual speed feedback.
Monitoring: Use ESP-IDF logs to track state, speed, and PWM duty cycle.
Optional Testing: Connect an LED to GPIO18 to visualize PWM output.

Project Structure


<img width="1334" height="297" alt="image" src="https://github.com/user-attachments/assets/610155fe-b942-4343-87c8-133694fbe7fb" />





Testing and Validation

Simulation Mode: Enable SIMULATE_ENCODER in encoder_sim.c to test without hardware. Verify logs for speed (400–1000 Hz) and duty cycle updates.
Real Encoder Mode: Disable SIMULATE_ENCODER, connect an encoder to GPIO19 (A) and GPIO21 (B), and verify pulse counts and speed in logs.
Fault Detection: Speeds >2000 Hz or <0 Hz trigger STATE_FAULT. Test by modifying encoder_sim_measure_speed in simulation mode.
Hardware Validation: Connect an LED to GPIO18 to visualize PWM output or use a multimeter to measure duty cycle.

PID Tuning
Fine-tune the PID controller in main.c for optimal performance:
pid_init(&pid, 0.1f, 0.01f, 0.0f, 0.01f, 0.0f, 100.0f); // Adjust Kp, Ki for faster response

Rebuild and test:
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor

Future Enhancements

Implement a timer-based main loop for precise control timing.
Add configurable GPIO pins and PID parameters via a configuration file.
Enhance fault detection for overcurrent or timeout conditions.
Support multiple encoder resolutions for broader hardware compatibility.

License
This project is licensed under the MIT License. See LICENSE for details.
Contributing
Contributions are welcome! Please open issues or pull requests for bug fixes, feature enhancements, or hardware integrations. This project is designed for learning and can be extended for advanced motor control applications.
Contact
Created by Gaurav Kumar. Connect with me on GitHub for questions or collaboration.
