# ESP Audio Development Kits hardware 

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

This is hardware design files for ESP32 Audio Development Kit

![image](https://user-images.githubusercontent.com/5459747/158466512-df669764-af58-40e1-a43f-441dc81c687c.png)

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
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/379863e4-a8a8-427d-8005-0649c9b06190) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/481cb005-b5fe-4d21-91e8-8c668a99703b) External RGB Strip connector. Chained to built-in RGB LED <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/257defe9-0bd0-47e8-a04d-714de224c25b) External Power switch. Short via external button to power on the board. <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/fe734f57-6319-49c0-9669-c51af540061e) External 5V source. Alternative source of power and charging <br /> ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/d3a84bff-7c0e-4efe-9dc4-0ec4bf5a0631) Disable battery current protection jumper. Short to override built-in 2A current limiter (only if you know what you're doing)

### GAIN and MONO Selection

Both Boards have jumpers that can be used to configure gain factor individually for both channels. Default GAIN is +9Db. Short appropriate jumper to override default
To hardwire DAC into MONO mode you need to cut MONO jumper (it is short by default)

| Loud-ESP MINI       | Loud-ESP    |
|-------------|----------------|
| ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/15e9eba4-132a-4f93-954e-a971454938a3) | ![image](https://github.com/sonocotta/esp32-audio-development-kit/assets/5459747/a5e39b89-b887-44aa-afdb-ee2242780313) | 


## Board Revisions

These boards comes in multiple revisions, normally revision of the specific board will be clearly visible on the back side of the PCB.

!rev img

Current distribution of board revisions looks like this

| Revision | Audio Development Kit | Revision | Audio Development Kit Mini  |
|----|----|----|----|
| A |  | | |
| B |  | | |
| C |  | | |
| D |  | | |
| E |  | | |

## Pin Definitions

Generally pins will differ slightly from revision to revision, and will be defined in platformio.ini config file. Most essential ones are listed below

Each usable pin is subscribed directly on the PCB, like on exmaple below

!img

### ESP8266 Development Boards

### ESP32 Development Boards

| PIN DEF | IO | G, H | J
|----|----|----|----|
|  | | | |
