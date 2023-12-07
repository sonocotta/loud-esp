# Loud-ESP

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)


![image](https://user-images.githubusercontent.com/5459747/158466253-0855f9ef-9246-4e30-b53f-b1fed834d44e.png)

Loud-ESP Kits is a set of development boards designed to make audio development for ESP platform easy and feature-rich.

Key properties of this kit
- It uses ESP chips as a MCU core, which has huge community support and adaptation
- It delivers 2 channel amplified high quality audio ready to be connected to speakers of choice - no need to have external amplifier. Therefore it is all you need to complete your audio projecs
- It has low power consumption and built-in Li-Ion battery management - therefore it is a good choice for any sort of a portable and wearable audio projects.
- It has a rich UX peripheral, most obviously FTF screen with touch, but also IR receiver, and header for physical controls, like rotary encoder or joystick. Therefore it is a good choice for standalone user-facing audio projects.

## Loud-ESP Features

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
  - Onboard 5V boost converter
  
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




## Software samples

Please find more information in the [firmware](/firmware) section

### Platformio IDE
 
All samples are provided as [Plarformio IDE](https://platformio.org/platformio-ide) projects. After installing it, open sample project. Select proper environment based on your dock. Run `Build` and `Upload` commands to install necessary tools and libraries, build and upload prject to the board. Communication and proper upload method selection will be handled by IDE automatically. 

## Arduino IDE

Follow the [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) library guide. Default settings will work out of the box with ESP8266 and ESP32 boards. For ESP32C3 and ESP32S2 board please adjust pinout according to the above section

## Squeezelite-ESP32

Squeezelite-ESP32 is a multimedia software suite, that started as renderer (or player) of LMS (Logitech Media Server). Now it is extended with 
- **Spotify** over-the-air player using SpotifyConnect (thanks to cspot)
- **AirPlay** controller (iPhone, iTunes ...) and enjoy synchronization multiroom as well (although it's AirPlay 1 only)
- Traditional **Bluetooth** device (iPhone, Android)

And LMS itself
- Streams your local music and connect to all major on-line music providers (Spotify, Deezer, Tidal, Qobuz) using Logitech Media Server - a.k.a LMS with **multi-room** audio synchronization.
- LMS can be extended by numerous plugins and can be controlled using a Web browser or dedicated applications (iPhone, Android).
- It can also send audio to UPnP, Sonos, ChromeCast and AirPlay speakers/devices.

All ESP32 based boards are tested with [Squeezelite-ESP32](https://github.com/sle118/squeezelite-esp32) software, that can be flashed using nothing but web-browser. You can use [Squeezelite-ESP32 installer](https://sle118.github.io/squeezelite-esp32-installer/) for that purpose.

### How to flash and configure ("ESP Audio Duo", "HiFi-ESP" and "Louder ESP")

Use [Squeezelite-ESP32 installer](https://sle118.github.io/squeezelite-esp32-installer/) to flash firmware first. 

|   |   |
|---|---|
| Use `Generic/I2S` profile | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/134eea60-149b-48d6-90cb-e68b421b61b2) |  
| Connect device to USB port and select if from the list | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/5cb87b71-ff9c-4aa6-acdc-602b9c918e67) ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/38a313c6-5b38-4b63-9e4b-209ffd960c6b)| 
| Press `Flash` and wait around 2 minutes  | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/e93f2106-b6e2-4625-92d1-a906c8375fd7) | 
| (Optional) You may enter serial console to get more information | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/2d9c3b58-f4bf-49a5-8c6b-fc12c8cc8d10) |
| Device is in recovery mode. Connect to `squeezelite-299fac` wifi network with `squeezelite` password (your network name suffix will be different) | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/2763663c-dbc9-4c71-af12-0a6fb9c2d94d) |
| When redirected to captive portal let device scan wifi network and provide valid credentials | <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/d2540ffb-d1d1-4441-a2b1-bbd6b8ad608f" width="30%" /> <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/b21f30e6-8899-46bc-b047-23281cae52b8" width="30%" /> <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/5dd1a1f6-0c6d-4045-b135-1d8cdd077161" width="30%" /> |
| You can use provided IP address (http://192.168.1.99/ on the screenshot) to access settings page |  ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/e3bbf910-1a5c-4c58-bd4e-c348ef0a91e5)
| Navigate to **Hardware** section and provide following updates in the **DAC Options** <br/><br /> DAC model: `I2S` <br/> Clock GPIO: `26` <br/> Word Select GPIO: `25` <br/> Data GPIO: `22` <br/> <br /> Press `Save` button |  ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/f52e3a05-abe6-40d7-98ac-212bff6c431f)
| (Optional) You may change device names to something close to you heart| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/adc1fff1-8572-4fe5-9d88-943d86a3fb11)
| Exit recovery | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/c54b18fa-c995-4e67-bced-820f8bce5fe6)
  
You can use it now
| Bluetooth  | Spotify Connect  | AirPlay | LMS Renderer  |
|---|---|---|---|
| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/cd0e7cb2-4a15-48fc-b308-0281e414619e)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/edcb5a3b-bead-44d8-b51d-4c36ed19b7da)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/20586bb4-bc51-4cfb-802a-c6072987c1da)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/dfdb89dd-755b-42fe-a381-a92011f9c681)

## Where to buy

You may support our work by ordering this product at [Tindie](https://www.tindie.com/products/sonocotta/loud-esp/)
