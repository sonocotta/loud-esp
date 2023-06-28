# ESP Audio Development Kits firmware

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

...

## Introduction

Most repos are [Platformio](https://platformio.org/) projects, all tested under Ubuntu 20.04 LTS. Some are [ESP IDF](https://www.espressif.com/en/products/sdks/esp-idf) projects, and would require [installation steps](https://github.com/espressif/esp-idf) before build

Most of the repos will utilize multiple environments with different settings for different board revisions. Please refer to **platformio.ini** file within project folder.

For each project please find detailed description using links below

## ESP Audio Development Kits repos

- (WIP) [esp-tunein-player](/firmware/esp-tunein-player) - TuneIn Wifi client
- [test-audio](/firmware/test-audio)
    - [esp32-esp8266-audio-test-eeprom](/firmware/test-audio/esp32-esp8266-audio-test-eeprom) - play audio from EEPROM example
    - [esp32-esp8266-audio-test-spiffs](/firmware/test-audio/esp32-esp8266-audio-test-spiffs) - play audio from SPIFFS in loop example
    - [esp32-esp8266-audio-test-spiffs-once](/firmware/test-audio/esp32-esp8266-audio-test-spiffs-once) - play audio from SPIFFS once example
    - [esp32-esp8266-audio-test-spiffs-random](/firmware/test-audio/esp32-esp8266-audio-test-spiffs-random) - play audio from SPIFFS randomly example
    - [esp32-esp8266-audio-test-stream](/firmware/test-audio/esp32-esp8266-audio-test-stream) - play audio from internet stream
    - [esp32-esp8266-audio-test-stream-web](/firmware/test-audio/esp32-esp8266-audio-test-stream-web) - play audio from internet stream with web UI
    - [esp32-esp8266-wifi-radio-station](/firmware/test-audio/esp32-esp8266-wifi-radio-station) - adaptation of [VolosR/TTGOInternetStation](https://github.com/VolosR/TTGOInternetStation) repo
- [test-peripheral](/firmware/test-peripheral)
    - [esp32-arduinomenu](/firmware/test-peripheral/esp32-arduinomenu) - example using [ArduinoMenu](https://github.com/neu-rah/ArduinoMenu) library with rotary encoder and joystick
    - [esp32-arduinomenu-dynamic](/firmware/test-peripheral/esp32-arduinomenu-dynamic) - same as above but with runtime built menu

    - [esp32-esp8266-peripheral-test](/firmware/test-peripheral/esp32-esp8266-peripheral-test) - test firmware that checks all available peripheral

    - [esp32-esp8266-test-batt-voltage-read](/firmware/test-peripheral/esp32-esp8266-test-batt-voltage-read) - example reading battery voltage
    - [esp32-test-sdcard](/firmware/test-peripheral/esp32-test-sdcard) - test reading files from SDCARD
    - [esp32-test-touch](/firmware/test-peripheral/esp32-test-touch) - test reading uilt-in touch sensors on pins
- [test-visual](/firmware/test-visual)
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
- [third-party-forks](/firmware/third-party-forks)
    - [esp32-cspot](/firmware/third-party-forks/esp32-cspot) - adaptation of [feelfreelinux/cspot](https://github.com/feelfreelinux/cspot) repo
    - [esp32-mp3-decoder](/firmware/third-party-forks/esp32-mp3-decoder) - adaptation of [MrBuddyCasino/ESP32_MP3_Decoder](https://github.com/MrBuddyCasino/ESP32_MP3_Decoder) repo
    - [Ka-Radio32](/firmware/third-party-forks/Ka-Radio32) - adaptation of [karawin/Ka-Radio32](https://github.com/karawin/Ka-Radio32) repo

## How to flash

### Recomended: Using Platformio

Whenever repo is a [Platformio](https://platformio.org/) project, it is preferred to use it to flash firmware to the board. **platformio.ini** file will contain specific sections for different board revisions and configurations. Hardware cpecific details is preferred to keep in this file as well.

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
