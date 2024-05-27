# Loud-ESP

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)
<a href="https://www.tindie.com/stores/sonocotta/?ref=offsite_badges&utm_source=sellers_andrey-malyshenko&utm_medium=badges&utm_campaign=badge_medium"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-mediums.png" alt="I sell on Tindie" width="150" height="78"></a>
<br />
[![Dev Chat](https://img.shields.io/discord/1233306441469657140?logo=discord&label=discord&style=flat-square)](https://discord.gg/PtnaAaQMpS)

![image](https://user-images.githubusercontent.com/5459747/158466253-0855f9ef-9246-4e30-b53f-b1fed834d44e.png)

Loud-ESP Kits is a set of development boards designed to make audio development for the ESP platform easy and feature-rich.

Key properties of this kit
- It uses ESP chips as an MCU core, which has huge community support and adaptation
- It delivers 2-channel amplified high-quality audio ready to be connected to speakers of choice - no need to have an external amplifier. Therefore it is all you need to complete your audio projects
- It has low power consumption and built-in Li-Ion battery management - therefore it is a good choice for any sort of portable and wearable audio projects.
- It has a rich UX peripheral, most obviously a TFT screen with touch, but also an IR receiver, and header for physical controls, like a rotary encoder or joystick. Therefore it is a good choice for standalone user-facing audio projects.

## Loud-ESP Features

MCU Core
  - ESP32 Dual Core 32-bit LX6 microprocessor running at 240 MHz
  - 16MB of flash storage
  - 8MB of PSRAM
  - CP2102 Serial communication/ Flashing chip

Battery management
  - Li-Ion battery charging with charge status indication
  - Battery overcharge and over-discharge protection
  - Battery overcurrent protection
  - Onboard 14500 battery holder or 2-pin connector for external battery (except Mini version)
  - Battery voltage reading using built-in DAC
  - Onboard power switch

Audio capabilities
  - Dual I2S stereo DAC (MAX98537) with built-in D-Class amp
  - 3W per channel on 4 Ohm load
  - Up to 92% efficiency
  - Onboard 5V boost converter
  
Peripheral
  - 2.8'' 320x240 TFT Screen with Touch sensor (except Mini version)
  - Wifi and BT
  - IR reader (except the Mini version)
  - RGB led, RGB LED strip header
  - SDCard socket (except Mini version)
  - JTAG header (except the Mini version)
  - Rotary encoder/Joystick header (except the Mini version)

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
 
All samples are provided as [Plarformio IDE](https://platformio.org/platformio-ide) projects. After installing it, open the sample project. Select the proper environment based on your dock. Run the `Build` and `Upload` commands to install necessary tools and libraries, and build and upload the project to the board. Communication and proper upload method selection will be handled by IDE automatically. 

## Arduino IDE

Follow the [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) library guide. Default settings will work out of the box with ESP8266 and ESP32 boards. For ESP32C3 and ESP32S2 board please adjust pinout according to the above section

## Squeezelite-ESP32

Squeezelite-ESP32 is a multimedia software suite, that started as a renderer (or player) of LMS (Logitech Media Server). Now it is extended with 
- **Spotify** over-the-air player using SpotifyConnect (thanks to cspot)
- **AirPlay** controller (iPhone, iTunes ...) and enjoy synchronization multiroom as well (although it's AirPlay 1 only)
- Traditional **Bluetooth** device (iPhone, Android)

And LMS itself
- Streams your local music and connects to all major online music providers (Spotify, Deezer, Tidal, Qobuz) using Logitech Media Server - a.k.a LMS with **multi-room** audio synchronization.
- LMS can be extended by numerous plugins and can be controlled using a Web browser or dedicated applications (iPhone, Android).
- It can also send audio to UPnP, Sonos, Chromecast, and AirPlay speakers/devices.

All ESP32-based boards are tested with [Squeezelite-ESP32](https://github.com/sle118/squeezelite-esp32) software, which can be flashed using nothing but web browser. You can use [Squeezelite-ESP32 installer](https://sonocotta.github.io/esp32-audio-dock/) for that purpose.


### How to flash and configure

Use [Installer for ESP Audio Dock](https://sonocotta.github.io/esp32-audio-dock/) to flash firmware first. It has been preconfigured to work with ESP Audio boards and will configure all hardware automatically.

|   |   |
|---|---|
| Select the correct device first	 | ![image](https://github.com/sonocotta/loud-esp/assets/5459747/078f00d6-5715-4c92-9ee3-7d3f01107d44)
| Connect the device to the USB port and select it from the list | ![image](https://github.com/sonocotta/loud-esp/assets/5459747/0148e157-60ed-4fbb-b05e-c671f0130bf8)
| Press `Flash` and wait around 2 minutes  | ![image](https://github.com/sonocotta/loud-esp/assets/5459747/c4ecec9d-5117-4dab-87ea-864a94fce609)
| (Optional) You may enter the serial console to get more information | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/89832ed4-4314-486f-b6a3-66efb46d3587)
| Device is in recovery mode. Connect to `squeezelite-299fac` wifi network with `squeezelite` password (your network name suffix will be different) | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/2763663c-dbc9-4c71-af12-0a6fb9c2d94d) |
| When redirected to the captive portal let the device scan wifi network and provide valid credentials | <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/d2540ffb-d1d1-4441-a2b1-bbd6b8ad608f" width="30%" /> <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/b21f30e6-8899-46bc-b047-23281cae52b8" width="30%" /> <img src="https://github.com/sonocotta/esp32-audio-dock/assets/5459747/5dd1a1f6-0c6d-4045-b135-1d8cdd077161" width="30%" /> |
| You can use provided IP address (http://192.168.1.99/ on the screenshot) to access settings page |  ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/e3bbf910-1a5c-4c58-bd4e-c348ef0a91e5)
| (Optional) You may change device names to something close to your heart| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/962c2ced-c7c4-4a8a-b58f-72ad2f1f6b94)
| Exit recovery | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/0fa0bab1-c225-47c7-ab26-29d9eb98ffcf)

  
You can use it now
| Bluetooth  | Spotify Connect  | AirPlay | LMS Renderer  |
|---|---|---|---|
| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/cd0e7cb2-4a15-48fc-b308-0281e414619e)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/edcb5a3b-bead-44d8-b51d-4c36ed19b7da)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/20586bb4-bc51-4cfb-802a-c6072987c1da)| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/dfdb89dd-755b-42fe-a381-a92011f9c681)

## Where to buy

You may support our work by ordering this product at [Tindie](https://www.tindie.com/products/sonocotta/loud-esp/) and [Elecrow](https://www.elecrow.com/loud-esp.html)
