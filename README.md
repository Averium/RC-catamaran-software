# Dual-Motor RC Catamaran – ESP32 Firmware & Remote Controller

This project contains the firmware for a dual-motor RC catamaran and its handheld remote controller.  
Both systems run on ESP32 microcontrollers and communicate wirelessly to provide smooth throttle control, steering, and system feedback.

## Features
- Independent BLDC control for left & right motors  
- ESC calibration and soft-ramp throttle control  
- Wireless bidirectional communication  
- Remote controller with joystick input and status display  
- Modular, extensible, object-oriented C code structure

## Structure
- `software/` – Catamaran firmware  
- `remote/` – Remote controller firmware  
- `components/` – Shared modules (PWM, ADC, motor drivers, utilities)  

## License
MIT
