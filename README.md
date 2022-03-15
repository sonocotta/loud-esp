# ESP Audio Development Kits

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

![image](https://user-images.githubusercontent.com/5459747/158466253-0855f9ef-9246-4e30-b53f-b1fed834d44e.png)

ESP Audio Development Kits is a set of development boards designed to make audio development for ESP platform easy and feature-rich.

Key properties of this kit
- It uses ESP chips as a MCU core, which has huge community support and adaptation
- It delivers 2 channel amplified high quality audio ready to be connected to speakers of choice - no need to have external amplifier. Therefore it is all you need to complete your audio projecs
- It has low power consumption and built-in Li-Ion battery management - therefore it is a good choice for any sort of a portable and wearable audio projects.
- It has a rich UX peripheral, most obviously FTF screen with touch, but also IR receiver, and header for physical controls, like rotary encoder or joystick. Therefore it is a good choice for standalone user-facing audio projects.

## ESP Audio Development Features

MCU Core
  - ESP32 Dual Core 32-bit LX6 microprocessor running at 240 MHz
  - 16MB of flash storage
  - 8MB of PSRAM
  - CP2102 Serial communication/ Flashing chip

Battery management
  - Li-Ion battery charging with charge status indication
  - Battery overcharge and overdischarge protection
  - Battery overcurrent protection
  - Onboard 14500 battery holder or 2-pin connector for external battery (except Mini version)
  - Battery voltage reading using built-in DAC
  - Onboard power switch

Audio capabilities
  - Dual I2S stereo DAC (MAX98537) with built in D-Class amp
  - 3W per channel on 4 Ohm load
  - Up to 92% efficiency
  - Onboard 5,5V boost converter
  
Peripheral
  - 2.8'' 320x240 TFT Screen with Touch sensor (except Mini version)
  - Wifi and BT
  - IR reader (except Mini version)
  - RGB led, RGB LED strip header
  - SDCard socket (except Mini version)
  - JTAG header (except Mini version)
  - Rotary encoder/Joystick header (except Mini version)

Other
  - RESET and GPIO0 (FLASH) buttons
  - Raspberry Pi 4 case mechanical compatibility
  - Software samples for every piece of onboard peripheral

## Hardware

Please find more information in the [hardware](/hardware) section

## Firmware

Please find more information in the [firmware](/firmware) section

## Where to buy

Crowd-supply campaign is being prepared at the moment
