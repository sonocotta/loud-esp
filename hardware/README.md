# ESP Audio Development Kits hardware 

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

These are the hardware design files for the ESP32 Audio Development Kit

**Prototype**

![image](https://user-images.githubusercontent.com/5459747/158466512-df669764-af58-40e1-a43f-441dc81c687c.png)

**Latest board**

| | | 
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
To hardwire DAC into MONO mode, you need to cut the MONO jumper (it is short by default)

| Loud-ESP MINI       | Loud-ESP    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/15e9eba4-132a-4f93-954e-a971454938a3) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/a5e39b89-b887-44aa-afdb-ee2242780313) | 


## Board Revisions

These boards comes in multiple revisions, normally the revision of the specific board will be clearly visible on the back side of the PCB.

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

[esp32-peripheral-test](../firmware/test-peripheral/esp32-peripheral-test) firmware demonstrates how to use each of available peripheral. For many of them, you would need libraries, which are also included in this example. Of couse many alternatives can be found, example will list those that are tested and work without a hustle. 
