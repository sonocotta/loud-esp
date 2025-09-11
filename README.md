# Loud-ESP development board

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)
<a href="https://www.tindie.com/stores/sonocotta/?ref=offsite_badges&utm_source=sellers_andrey-malyshenko&utm_medium=badges&utm_campaign=badge_medium"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-mediums.png" alt="I sell on Tindie" width="150" height="78"></a>
<br />
[![Dev Chat](https://img.shields.io/discord/1233306441469657140?logo=discord&label=discord&style=flat-square)](https://discord.gg/PtnaAaQMpS)

<img width="1587" height="1002" alt="image" src="https://github.com/user-attachments/assets/fb6cd736-475e-430b-b656-0f7bd52e3ca6" />

- [Loud-ESP development board](#loud-esp-development-board)
  - [Loud-ESP Features](#loud-esp-features)
  - [RFID Edition](#rfid-edition)
- [Hardware](#hardware)
  - [Features](#features)
    - [Loud-ESP Mini](#loud-esp-mini)
    - [Loud-ESP](#loud-esp)
      - [External connectors](#external-connectors)
    - [GAIN and MONO Selection](#gain-and-mono-selection)
  - [Board Revisions](#board-revisions)
  - [Pin Definitions](#pin-definitions)
    - [Pin usage summary](#pin-usage-summary)
    - [Rev H update](#rev-h-update)
      - [SD-CARD issues](#sd-card-issues)
      - [Updated pinout](#updated-pinout)
    - [How to use a specific peripheral](#how-to-use-a-specific-peripheral)
- [Firmware](#firmware)
  - [Introduction](#introduction)
  - [ESP Audio Development Kits repos](#esp-audio-development-kits-repos)
  - [How to flash](#how-to-flash)
    - [Recommended: Using Platformio](#recommended-using-platformio)
    - [Arduino IDE](#arduino-ide)
  - [ESPHome and Home Assistant](#esphome-and-home-assistant)
    - [Loud ESP Mini and Full version](#loud-esp-mini-and-full-version)
    - [Loud ESP Full version only](#loud-esp-full-version-only)
    - [Project example - Mopidy monitor](#project-example---mopidy-monitor)
    - [Bonus - automation example](#bonus---automation-example)
  - [Squeezelite-ESP32](#squeezelite-esp32)
    - [How to flash and configure](#how-to-flash-and-configure)
  - [Where to buy](#where-to-buy)


Loud-ESP Kits is a set of development boards designed to make audio development for the ESP platform easy and feature-rich.

Key properties of this kit
- It uses ESP chips as an MCU core, which has a huge community support and adaptation
- It delivers 2-channel amplified high-quality audio ready to be connected to speakers of choice - no need to have an external amplifier. Therefore, it is all you need to complete your audio projects
- It has low idle power consumption and built-in Li-Ion battery management; therefore, it is a good choice for any sort of portable and portable audio projects.
- It has a rich UX peripheral, most obviously a TFT screen with touch, but also an IR receiver, and a header for physical controls, like a rotary encoder or joystick. Therefore, it is a good choice for standalone user-facing audio projects.

As of autumn 2025, I'm working on a spin-off of the original board that is tailored specifically for [Espuino](https://github.com/biologist79/ESPuino) firmware, with a key difference to the original board being a replacement of the TFT screen with an RFID reader. It can be used for offline RFID-controlled media player devices, although I personally found it most usable in connection with Home Assistant and ESPHome driver integration. 

While working on the new revision of the board, both received a few major schematic improvements to address a few issues I found over time with the original design. These include

- New 3.3V buck-boost IC (TPS63060DSCR) that better handles low battery state
- SDIO interface for the SD-card (as opposed to SPI, it uses fewer pins and provides better speed)
- Move to the CH340C serial chip, as it seems more robust compared to the CP2102
- Better power switch and a few smaller improvements.

## Loud-ESP Features

<img width="1978" height="1312" alt="image" src="https://github.com/user-attachments/assets/46f90570-3364-401d-b5cd-80301260fd21" />

MCU Core
  - ESP32 Dual Core 32-bit LX6 microprocessor running at 240 MHz
  - 16MB of flash storage
  - 8MB of PSRAM
  - CP2102/CH340C Serial communication/ Flashing chip

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
  - RGB LED, RGB LED strip header
  - SDCard socket (except Mini version)
  - JTAG header (except the Mini version)
  - Rotary encoder/Joystick header (except the Mini version)

Other
  - RESET and GPIO0 (FLASH) buttons
  - Raspberry Pi 4 case mechanical compatibility
  - Software samples for every piece of onboard peripheral

## RFID Edition

{image-placeholder}

Compared to the original board, this one clearly changed shape to be hosted in the speaker enclosure. Apart from that, changes include

MCU, Audio, and Battery management
  - Same as Loud-ESP

Peripheral
  - RFID reader 
  - RGB LED bar (12 LEDs)
  - SDCard socket (except Mini version)
  - Onboard rotary encoder with push button
  - 5x Push-buttons connected via GPIO expander
  - 1x Push-button (instead of a slide switch) to cut off power while not in use
  

# Hardware

**Prototype**

![image](https://user-images.githubusercontent.com/5459747/158466512-df669764-af58-40e1-a43f-441dc81c687c.png)

**Latest board**

| Assembed | PCB only | 
|---|---|
| ![DSC_0016](https://github.com/user-attachments/assets/4cfdd93a-4dac-4961-b5b7-8627eb4a27f2) | ![DSC_0019](https://github.com/user-attachments/assets/9cbfd257-b2f6-4a49-821e-5c3074a19a03)

## Features

### Loud-ESP Mini

| IMAGE       | DESCRIPTION    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/959b3b40-747f-4eec-b4be-255d641d1d28) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/b8b6512a-1f65-498a-aa82-84e13cb929b7) MCU (WROVER ESP32) <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/c749113c-8aac-44e3-8cbe-3d0281474635) RESET and IO0/FLASH buttons <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/ade99152-8268-4361-b3d3-dec5bf15d88b) Speaker Terminals <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/3993f8fc-8415-4731-a3b1-47721e6bd8e8) Li-Ion Battery Connector <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/acb1c3f2-cf78-4239-9ce2-a7bcb7811048) Power switch <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/d49bb1f4-3fe5-4388-b0ff-88d15dca2392) USB port (Flashing + Serial + Power + Charging) <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/3bdc9cf2-a49a-48d9-a61b-e4f48064af2b) RGB LED </br> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/2a6fc637-59f9-481f-ae56-5c39b1378941) 5V Boost converter

### Loud-ESP

| IMAGE       | DESCRIPTION    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/3289820c-9760-4454-b8b4-14c1b8dfdb6f) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/b8b6512a-1f65-498a-aa82-84e13cb929b7) MCU (WROVER ESP32) <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/c749113c-8aac-44e3-8cbe-3d0281474635) RESET and IO0/FLASH buttons <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/ade99152-8268-4361-b3d3-dec5bf15d88b) Speaker Terminals <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/3993f8fc-8415-4731-a3b1-47721e6bd8e8) Li-Ion Battery Connector <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/acb1c3f2-cf78-4239-9ce2-a7bcb7811048) Power switch <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/d49bb1f4-3fe5-4388-b0ff-88d15dca2392) USB port (Flashing + Serial + Power + Charging) <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/3bdc9cf2-a49a-48d9-a61b-e4f48064af2b) RGB LED </br> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/2a6fc637-59f9-481f-ae56-5c39b1378941) Dual 5V Boost converter <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/723861e3-57b5-48ae-a2d9-d2d0d03d72e5) IR Sensor <br/> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/a9281b1c-c12d-48f1-a94b-d815c1d52bf7) TFT Screen Connector <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/2e1a358c-6c82-403a-9a55-3f56389d369d) SD Card socket <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/2ff780df-1f97-480a-9946-61cb78c0e351) Rotary Encoder/Joystick module conector

#### External connectors

| IMAGE       | DESCRIPTION    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/379863e4-a8a8-427d-8005-0649c9b06190) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/481cb005-b5fe-4d21-91e8-8c668a99703b) External RGB Strip connector. Chained to built-in RGB LED <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/257defe9-0bd0-47e8-a04d-714de224c25b) External Power switch. Short via the external button to power on the board. <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/fe734f57-6319-49c0-9669-c51af540061e) External 5V source. Alternative source of power and charging <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/d3a84bff-7c0e-4efe-9dc4-0ec4bf5a0631) Disable battery current protection jumper. Short to override built-in 2A current limiter (only if you know what you're doing)

### GAIN and MONO Selection

Both Boards have jumpers that can be used to configure the gain factor individually for both channels. The default GAIN is +9 dB. Short appropriate jumper to override the default
To hardwire the DAC into MONO mode, you need to cut the MONO jumper (it is short by default)

| Loud-ESP MINI       | Loud-ESP    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/15e9eba4-132a-4f93-954e-a971454938a3) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/a5e39b89-b887-44aa-afdb-ee2242780313) | 


## Board Revisions

These boards come in multiple revisions; normally, the revision of the specific board will be clearly visible on the back side of the PCB.

![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/86b32c3e-dd69-4518-9668-39900a6002bd)

The current distribution of board revisions looks like this

| Revision | Loud-ESP | Revision | Loud-ESP Mini  | Description
|----|----|----|----|----| 
| D | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/7ddb571d-022a-41c2-9c8e-f08c979d7b53) | - |  | Extended PCB with built-in Rotary Encoder/ Joystick
| E | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/ae89a157-672c-4a27-9c29-734ca52663f4) | E | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/433b2886-ab8b-431b-9ec1-c9aa13f1615b) | Pre-production prototype. Distributed among beta-testers
| F | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/bb767277-9762-48ff-bad5-bfd10ee21f27) | F | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/5f299db2-525f-414b-b3ee-c937e3175b4c) | Crowd Supply distribution
| G | ![image](https://github.com/user-attachments/assets/a15ced19-5ed9-47c9-9eb7-e164eb71484c) | - | - | Engineering sample that replace serial bridge with CH340 (due to few CP2102 failures reported by customers)
| H | ![image](https://github.com/user-attachments/assets/e67960f1-b3a2-46a4-877a-e331167279f9) | - | - | Recent board update, introduce improved power topology, CH340 Serial bridge, USB-C port, and SDCARD over SDIO interface.


Reviosions A, B, and C were early prototypes, never distributed due to multiple hardware issues.

## Pin Definitions

![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/86551857-61ef-4873-a2aa-7812a9a34e7d)

### Pin usage summary

| PIN  | PRIMARY FUNC | JTAG | I2S  | TFT     | TOUCH_TFT | RGB_LED | BAT    | IR    | SDCARD | ROTARY ENC/ BTN INP | PSRAM     |
|------|--------------|------|------|---------|-----------|---------|--------|-------|--------|---------------------|-----------|
| IO0  | BOOT SEL     |      |      |         |           |         |        | IR_IN |        |                     |           |
| IO1  | TX           |      |      |         |           |         |        |       |        |                     |           |
| IO2  |              |      |      |         | CS        |         |        |       |        |                     |           |
| IO3  | RX           |      |      |         |           |         |        |       |        |                     |           |
| IO4  |              |      |      | DC      |           |         |        |       |        |                     |           |
| IO5  | VSPI_CS      |      |      | CS      |           |         |        |       |        |                     |           |
| IO12 | HSPI_MISO    | MTDI |      |         |           |         |        |       |        |                     |           |
| IO13 | HSPI_MOSI    | MTCK |      |         |           |         |        |       |        |                     |           |
| IO14 | HSPI_CLK     | MTMS |      |         |           |         |        |       |        |                     |           |
| IO15 | HSPI_CS      | MTDO |      |         |           |         |        |       |        |                     |           |
| IO16 |              |      |      |         |           |         |        |       |        |                     | PSRAM_CS  |
| IO17 |              |      |      |         |           |         |        |       |        |                     | PSRAM_CLK |
| IO18 | VSPI_CLK     |      |      | CLK     | CLK       |         |        |       | CLK    |                     |           |
| IO19 | VSPI_MISO    |      |      | MISO    | MISO      |         |        |       | MISO   |                     |           |
| IO21 |              |      |      |         |           |         | CHRG   |       |        |                     |           |
| IO22 |              |      | DATA |         |           |         |        |       |        |                     |           |
| IO23 | VSPI_MOSI    |      |      | MOSI    | MOSI      |         |        |       | MOSI   |                     |           |
| IO25 |              |      | WS   |         |           |         |        |       |        |                     |           |
| IO26 |              |      | CLK  |         |           |         |        |       |        |                     |           |
| IO27 |              |      |      |         |           |         |        |       | CS     |                     |           |
| IO32 |              |      |      | RES/LED |           |         |        |       |        |                     |           |
| IO33 |              |      |      |         |           | OUT     |        |       |        |                     |           |
| IO34 |              |      |      |         |           |         | BAT_IN |       |        |                     |           |
| IO35 |              |      |      |         |           |         |        |       |        | A                   |           |
| IO36 |              |      |      |         |           |         |        |       |        | B                   |           |
| IO39 |              |      |      |         |           |         |        |       |        | BTN                 |           |

### Rev H update

Key hardware updates introduced in the rev H

- SD-CARD over SDIo interface
- CH340 Serial chip instead of CP2102
  - Few people reported its failure
- USB-C instead of mini-USB
- TPS63060 step-up/step-down 3.3V converter, which ensures smooth operation with low battery levels
  - Previous revision started flickering when battery voltage goes low, since the 3V3 rail would go even below 3V, causing ESP32 brown-outs

#### SD-CARD issues

Having used Loud-ESP for a few projects involving SDCARD, I came to the idea that having 3 high-speed devices on the same SPI line is not the best approach. Since SD-CARD can alternatively be connected to the SDIO interface, I decided to follow that path in revision H.

SDIO interface is hardwired to pins IO2, IO4, IO12-15, and by the SD specification, **all must be pulled high** for reliable operation. At the same time, the ESP32 specification states that **pins 2 and 12 must be low at boot**, otherwise the ESP32 will go into download mode. Thus, I left them floating, which caused some SD cards to fail. I didn't find a reliable solution to this contradiction and consider going back to the SPI interface in the next revision.

#### Updated pinout

| PIN  | PRIMARY FUNC | JTAG | I2S  | TFT  | TOUCH_TFT | RGB_LED | BAT | IR    | SDCARD | ROTARY ENC/ BTN INP | PSRAM     |
|------|--------------|------|------|------|-----------|---------|-----|-------|--------|---------------------|-----------|
| IO0  | BOOT SEL     |      |      |      |           |         |     | IR_IN |        |                     |           |
| IO1  | TX           |      |      |      |           |         |     |       |        |                     |           |
| IO2  |              |      |      |      |           |         |     |       | D0     |                     |           |
| IO3  | RX           |      |      |      |           |         |     |       |        |                     |           |
| IO4  |              |      |      |      |           |         |     |       | D1     |                     |           |
| IO5  | VSPI_CS      |      |      | CS   |           |         |     |       |        |                     |           |
| IO12 | HSPI_MISO    | MTDI |      |      |           |         |     |       | D2     |                     |           |
| IO13 | HSPI_MOSI    | MTCK |      |      |           |         |     |       | D3     |                     |           |
| IO14 | HSPI_CLK     | MTMS |      |      |           |         |     |       | CLK    |                     |           |
| IO15 | HSPI_CS      | MTDO |      |      |           |         |     |       | CMD    |                     |           |
| IO16 |              |      |      |      |           |         |     |       |        |                     | PSRAM_CS  |
| IO17 |              |      |      |      |           |         |     |       |        |                     | PSRAM_CLK |
| IO18 | VSPI_CLK     |      |      | CLK  | CLK       |         |     |       | CLK    |                     |           |
| IO19 | VSPI_MISO    |      |      | MISO | MISO      |         |     |       | MISO   |                     |           |
| IO21 |              |      |      | DC   |           |         |     |       |        |                     |           |
| IO22 |              |      | DATA |      |           |         |     |       |        |                     |           |
| IO23 | VSPI_MOSI    |      |      | MOSI | MOSI      |         |     |       | MOSI   |                     |           |
| IO25 |              |      | WS   |      |           |         |     |       |        |                     |           |
| IO26 |              |      | CLK  |      |           |         |     |       |        |                     |           |
| IO27 |              |      |      |      | CS        |         |     |       |        |                     |           |
| IO32 |              |      |      | LED  |           |         |     |       |        |                     |           |
| IO33 |              |      |      |      |           | OUT     |     |       |        |                     |           |
| IO34 |              |      |      |      |           |         |     |       |        | B                   |           |
| IO35 |              |      |      |      |           |         |     |       |        | A                   |           |
| IO36 |              |      |      |      |           |         |     |       |        | BTN                 |           |
| IO39 |              |      |      |      |           |         |     |       | HOST   |                     |           |

### How to use a specific peripheral

[esp32-peripheral-test](../firmware/test-peripheral/esp32-peripheral-test) firmware demonstrates how to use each of available peripheral. For many of them, you would need libraries, which are also included in this example. Of course, many alternatives can be found; for example, we will list those that are tested and work without a hassle. 

# Firmware

## Introduction

Most repos are [Platformio](https://platformio.org/) projects, all tested under Ubuntu 20.04 LTS. Some are [ESP IDF](https://www.espressif.com/en/products/sdks/esp-idf) projects, and would require [installation steps](https://github.com/espressif/esp-idf) before build

Most of the repos will utilize multiple environments with different settings for different board revisions. Please refer to **platformio.ini** file within the project folder.

For each project, please find a detailed description using links below

## ESP Audio Development Kits repos

- (WIP) [esp-tunein-player](/firmware/esp-tunein-player) - TuneIn Wifi client
- [esp32-soundboard](/firmware/esp32-soundboard) - singing birds sound board
- [test-audio](/firmware/test-audio)
    - [esp32-esp8266-audio-test-eeprom](/firmware/test-audio/esp32-esp8266-audio-test-eeprom) - play audio from EEPROM example
    - [esp32-esp8266-audio-test-sdcard-random](/firmware/test-audio/esp32-esp8266-audio-test-sdcard-random) - play audio files from sd-card
    - [esp32-esp8266-audio-test-spiffs](/firmware/test-audio/esp32-esp8266-audio-test-spiffs) - play audio from SPIFFS in loop example
    - [esp32-esp8266-audio-test-spiffs-once](/firmware/test-audio/esp32-esp8266-audio-test-spiffs-once) - play audio from SPIFFS once example
    - [esp32-esp8266-audio-test-spiffs-random](/firmware/test-audio/esp32-esp8266-audio-test-spiffs-random) - play audio from SPIFFS randomly example
    - [esp32-esp8266-audio-test-stream](/firmware/test-audio/esp32-esp8266-audio-test-stream) - play audio from internet stream
    - [esp32-esp8266-audio-test-stream-web](/firmware/test-audio/esp32-esp8266-audio-test-stream-web) - play audio from internet stream with web UI
    - [esp32-esp8266-wifi-radio-station](/firmware/test-audio/esp32-esp8266-wifi-radio-station) - adaptation of [VolosR/TTGOInternetStation](https://github.com/VolosR/TTGOInternetStation) repo
    - [esp32-esp8266-talking-clock](/firmware/test-audio/esp32-esp8266-talking-clock) - Talking clock mini project
    - [esp32-esp8266-text-to-speach-sam](/firmware/test-audio/esp32-esp8266-text-to-speach-sam) - Offline text to speach generation via SAM
    - [esp32-i2s-bare](/firmware/test-audio/esp32-i2s-bare) - audio generation using bare I2S sampling
- [test-peripheral](/firmware/test-peripheral)
    - [esp32-arduinomenu](/firmware/test-peripheral/esp32-arduinomenu) - example using [ArduinoMenu](https://github.com/neu-rah/ArduinoMenu) library with rotary encoder and joystick
    - [esp32-arduinomenu-dynamic](/firmware/test-peripheral/esp32-arduinomenu-dynamic) - same as above but with runtime built menu
    - [esp32-peripheral-test](/firmware/test-peripheral/esp32-esp8266-peripheral-test) - test firmware that checks all available peripheral
    - [esp32-test-battery](/firmware/test-peripheral/esp32-esp8266-test-batt-voltage-read) - example reading battery voltage and charge status
    - [esp32-test-sdcard](/firmware/test-peripheral/esp32-test-sdcard) - test reading files from SDCARD
    - [esp32-test-sleep-by-pin](/firmware/test-peripheral/esp32-test-sleep-by-pin) - sleep mode with button triggered wake up
    - [esp32-test-sleep-by-timer](/firmware/test-peripheral/esp32-test-sleep-by-timer) - sleep mode with timer wake up
    - [esp32-test-touch-pins](/firmware/test-peripheral/esp32-test-touch-pins) - test reading built-in touch sensors on pins
    - [esp32-test-touch-screen](/firmware/test-peripheral/esp32-test-touch-screen) - test TFT touch sensor
- [test-visual](/firmware/test-visual)
    - [esp32-demo-ir-volume](/firmware/test-visual/esp32-demo-ir-volume) - demo of using IR controller to change volume
    - [esp32-demo-visualisations](/firmware/test-visual/esp32-demo-visualisations/) - port of classic visualisations
    - [esp32-demo-vu-meters](/firmware/test-visual/esp32-demo-vu-meters/) - demo of VU meters rendered on the screen
    - [esp32-esp8266-adafruit-spi-display-test](/firmware/test-visual/esp32-esp8266-adafruit-spi-display-test) - TFT example using Adafruit library
    - [esp32-esp8266-spi-display-test](/firmware/test-visual/esp32-esp8266-spi-display-test) - TFT example using [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library
    - [esp32-esp8266-currency-monitor](/firmware/test-visual/esp32-esp8266-currency-monitor) - adaptation of [VolosR/currency240240](https://github.com/VolosR/currency240240) repo
    - [esp32-esp8266-fake-win-xp](/firmware/test-visual/esp32-esp8266-fake-win-xp) - adaptation of [VolosR/fAKEWINESP](https://github.com/VolosR/fAKEWINESP) repo
    - [esp32-esp8266-screensaver](/firmware/test-visual/esp32-esp8266-screensaver) - 6 classic screensavers for ESP32 with TFT or VGA output
    - [esp32-esp8266-trex-game](/firmware/test-visual/esp32-esp8266-trex-game) - adaptation of [VolosR/TRexTTGOdisplay](https://github.com/VolosR/TRexTTGOdisplay) repo
    - [esp32-esp8266-weather-station](/firmware/test-visual/esp32-esp8266-weather-station) - adaptation of [VolosR/TTGOWeatherStation](https://github.com/VolosR/TTGOWeatherStation) repo
    - [esp32-esp8266-weather-station-color](/firmware/test-visual/esp32-esp8266-weather-station-color) - adaptation of [ThingPulse/esp8266-weather-station-color](https://github.com/ThingPulse/esp8266-weather-station-color) repo
    - [esp32-esp8266-wifi-meaters](/firmware/test-visual/esp32-esp8266-wifi-meaters) - adaptation of [davidchatting/ThreeWiFiMeters](https://github.com/davidchatting/ThreeWiFiMeters) repo
    - [esp32-spotify-screen](/firmware/test-visual/esp32-spotify-screen) - adaptation of [CosmicMac/M5Spot](https://github.com/CosmicMac/M5Spot) repo
    - [esp32-test-lvgl](/firmware/test-visual/esp32-test-lvgl) - LVGL usage example
- [third-party-forks](/firmware/third-party-forks)
    - [esp32-cspot](/firmware/third-party-forks/esp32-cspot) - adaptation of [feelfreelinux/cspot](https://github.com/feelfreelinux/cspot) repo
    - [ESP32-MiniWebRadio](/firmware/third-party-forks/ESP32-MiniWebRadio) - adaptation of [schreibfaul1/ESP32-MiniWebRadio](https://github.com/schreibfaul1/ESP32-MiniWebRadio) repo
    - [esp32-mp3-decoder](/firmware/third-party-forks/esp32-mp3-decoder) - adaptation of [MrBuddyCasino/ESP32_MP3_Decoder](https://github.com/MrBuddyCasino/ESP32_MP3_Decoder) repo
    - [Ka-Radio32](/firmware/third-party-forks/Ka-Radio32) - adaptation of [karawin/Ka-Radio32](https://github.com/karawin/Ka-Radio32) repo

## How to flash

### Recommended: Using Platformio

Whenever repo is a [Platformio](https://platformio.org/) project, it is preferred to use it to flash firmware to the board. **platformio.ini** file will contain specific sections for different board revisions and configurations. Hardware-specific details are preferred to keep in this file as well.

```
[env:esp32-dev-board-c-240x240]
platform = espressif32
board = esp32dev
framework = arduino
build_flags =
  -D USER_SETUP_LOADED=1
  -D ST7789_DRIVER
  -D TFT_WIDTH=240
  -D TFT_HEIGHT=240
  -D TFT_DC=27
  -D TFT_BL=26
  -D TFT_INVERSION_ON=1
  -D TFT_RGB_ORDER=TFT_BGR
  -D TFT_ROTATION=2

[env:esp32-dev-board-c-240x320]
platform = espressif32
board = esp32dev
build_flags =
  -D USER_SETUP_LOADED=1
  -D ST7789_DRIVER
  -D TFT_WIDTH=240
  -D TFT_HEIGHT=320
  -D TFT_DC=13
  -D TFT_CS=05
  -D TFT_BL=12
  -D TFT_INVERSION_OFF=1
  -D TFT_RGB_ORDER=TFT_BGR
  -D TFT_ROTATION=1
```

### Arduino IDE

Follow the [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio) library guide. Default settings will work out of the box with both Loud-ESP and Loud-ESP Mini boards. 


## ESPHome and Home Assistant

Being an ESP32-based device, you can easily integrate it into your Home Assistant using ESPHome. Start with [esphome web installer](https://web.esphome.io/), which will give you an ESPHome base install and WiFi configuration in minutes. 

|   |   |
|----|----|
| ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/abde451b-5619-4b32-a024-3f680999567f) | ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/fc567914-0d0a-4402-a5c5-ddfad77f2b83) |


Next, navigate to your Home Assistant (assuming you have your [ESPHome integration](https://esphome.io/guides/getting_started_hassio.html) installed), and adopt the newly created node

![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/299fb7ab-003e-4259-b0c1-9d13dd3e54ba)

[ESPHome](/firmware/esphome/) will give you ESPHome configs for both the mini and full versions of the board.

### Loud ESP Mini and Full version

The [loud-esp-mini](/firmware/esphome/loud-esp-mini.yaml) config enables common board peripheral

- `media_player` publishes the media player into the Home Assistant, so you can use it together with the native player or Music Assistant. You have a volume knob in the HA as well.
- ![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/f73a7347-672f-4dfc-afb8-35c7eb78e464)
- Volume set up to 50% on player start. You can change that
- `light` component exposes onboard RGB LED for use in integrations. In this example, it lights an LED depending on the `media_player` state
- `psram` enabled for all devices to smooth out playback

### Loud ESP Full version only

The [loud-esp](/firmware/esphome/loud-esp.yaml) config enables all the board peripheral

- `remote_receiver` exposes the IR reader. Example configures the SAMSUNG TV remote to control volume using volume buttons. 
- `display` component configures the onboard display 
- `touchscreen` component configures the onboard touchscreen and dumps touches to the console

### Project example - Mopidy monitor

The [loud-esp-mopidy-monitor](/firmware/esphome/loud-esp-mopidy-monitor.yaml) config is a project that I did myself for HA [Mopidy](https://mopidy.com/) media center integration

- `media_player` can be used for voice announcements
- `display` shows the current Mopidy playing item, Artist, and title.
- three playback buttons allow Play/Pause track and move back and forward along the playlist via `touchscreen` controls
- on the top of `display`, there is a volume indicator with two buttons to change it
- The progress bar shows the current progress of the track
- `display` also shows playlist progress on the bottom with the current item highlighted 

![image](https://github.com/sonocotta/loud-esp/assets/5459747/928184dc-bfa7-4456-a7f9-c12743d5ccc9)

Currently, there is no component that can display track images, but it seems like it [will be added](https://github.com/esphome/esphome/pull/4710/) soon

### Bonus - automation example

The true power of the native speaker in the eHA is the use of automation. One example that I find useful. This simple automation will be pronounced every hour between 8 AM and 9 PM. Another one is used to pronounce bedtime, you get the point...

![image](https://github.com/sonocotta/esp32-audio-dock/assets/5459747/fd25fb6a-19a7-4957-a2cb-a723723fde4d)

## Squeezelite-ESP32

Squeezelite-ESP32 is a multimedia software suite that started as a renderer (or player) of LMS (Logitech Media Server). Now it is extended with 
- **Spotify** over-the-air player using SpotifyConnect (thanks to cspot)
- **AirPlay** controller (iPhone, iTunes ...) and enjoy synchronization multiroom as well (although it's AirPlay 1 only)
- Traditional **Bluetooth** device (iPhone, Android)

And LMS itself
- Streams your local music and connects to all major online music providers (Spotify, Deezer, Tidal, Qobuz) using Logitech Media Server - a.k.a LMS with **multi-room** audio synchronization.
- LMS can be extended by numerous plugins and can be controlled using a Web browser or dedicated applications (iPhone, Android).
- It can also send audio to UPnP, Sonos, Chromecast, and AirPlay speakers/devices.

All ESP32-based boards are tested with [Squeezelite-ESP32](https://github.com/sle118/squeezelite-esp32) software, which can be flashed using nothing but a web browser. You can use [Squeezelite-ESP32 installer](https://sonocotta.github.io/esp32-audio-dock/) for that purpose.

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
