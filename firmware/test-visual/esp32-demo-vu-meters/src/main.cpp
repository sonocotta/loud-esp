#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifdef TEST_TFT
#include "SPI.h"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef TEST_RGB
#include <FastLED.h>
#define NUM_LEDS 9
CRGB leds[NUM_LEDS];
#endif

#ifdef TEST_ROTARY_ENC
#include <ESP32Encoder.h>
ESP32Encoder encoder;
#endif

#define NEEDLE_ANGLE_MAX 100

#include "visualisations/Visualiser.h"
#include "visualisations/SerialVisualiser.h"
#include "visualisations/TextVisualiser.h"
#include "visualisations/BufferedVisualiser.h"

#include "visualisations/DigitalVisualiser.h"
#include "visualisations/AnalogVisualiser.h"
#include "visualisations/BarsVisualiser.h"

// samplebuffers
#define samplesCount 2048
int16_t samplesLeft[samplesCount] = {0};
int16_t samplesRight[samplesCount] = {0};

Visualiser *visualisers[] = {
    //    new SerialVisualiser(&tft),
    //    new TextVisualiser(&tft),
    //    new BufferedVisualiser(&tft)
    //    new DigitalVisualiser(&tft)
    //    new AnalogVisualiser(&tft)
    new BarsVisualiser(&tft)
};

void setup()
{
    Serial.begin(SERIAL_BAUD);

#ifdef TEST_ROTARY_ENC
    pinMode(TEST_ROTARY_ENC, INPUT);
    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoder.attachHalfQuad(TEST_ROTARY_ENC_A, TEST_ROTARY_ENC_B);
#endif

#ifdef TEST_JOYSTICK
    pinMode(TEST_JOYSTICK_BTN, INPUT_PULLUP);
#endif

#ifdef TEST_RGB
    FastLED.addLeds<WS2812B, TEST_RGB, GRB>(leds, NUM_LEDS);

    leds[0] = CRGB::Red;
    FastLED.show();
    delay(250);
    leds[0] = CRGB::Black;
    FastLED.show();
#endif

    tft.begin();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);

#ifdef TFT_INVERT
    tft.invertDisplay(true);
#endif
    tft.setSwapBytes(true);

    for (uint8_t i = 0; i < sizeof(visualisers) / sizeof(Visualiser *); i++)
    {
        visualisers[i]->init();
    }
}

int16_t sampleRight, sampleLeft = 0;
int16_t saved_angle = 0;
uint8_t button_prev_state = 0;
int16_t encoder_prev_state = 0;

void loop()
{
    static uint32_t samplesCounter = 0;

#ifdef TEST_ROTARY_ENC
    int16_t encRead = encoder.getCount();
    if (encoder_prev_state != encRead)
    {
        Serial.printf("Encoder reads: %d\n", encRead);
        sampleLeft = encRead << 4;
        sampleRight = encRead << 4;
        encoder_prev_state = encRead;
    }

    // auto btnRead = digitalRead(TEST_ROTARY_ENC);
    // if (button_prev_state != btnRead)
    // {
    //     Serial.printf("Encoder button reads: %s\n", btnRead == 1 ? "high" : "low");
    //     if (btnRead == HIGH)
    //     {
    //         saved_angle = angle;
    //         angle = NEEDLE_ANGLE_MAX;
    //     }
    //     else
    //     {
    //         angle = saved_angle;
    //     }
    //     button_prev_state = btnRead;
    // }

#ifdef TEST_RGB
    uint8_t ledsCount = map(angle, 0, NEEDLE_ANGLE_MAX, 1, NUM_LEDS);
    uint8_t intensity = map(angle, 0, NEEDLE_ANGLE_MAX, 32, 255);
    auto color = (CRGB){intensity, 255 - intensity, 0};
    for (uint8_t i = 1; i < NUM_LEDS; i++)
        leds[i] = (i < ledsCount ? color : CRGB::Black);
    FastLED.show();
#endif
#endif

#ifdef TEST_JOYSTICK
    uint32_t x = analogRead(TEST_JOYSTICK_A) >> 2;
    uint32_t y = analogRead(TEST_JOYSTICK_B) >> 2;
    Serial.printf("Joystick readings: %d, %d, %d\n", y, x, digitalRead(TEST_JOYSTICK_BTN));
    sampleLeft = x << 4;
    sampleRight = y << 4;
#ifdef TEST_RGB
    uint8_t ledsCount = map(y, 0, 1023, 1, NUM_LEDS);
    uint8_t intensity = map(y, 0, 1023, 32, 255);
    auto color = (CRGB){intensity, 255 - intensity, 0};
    for (uint8_t i = 1; i < NUM_LEDS; i++)
        leds[i] = (i < ledsCount ? color : CRGB::Black);
    FastLED.show();
#endif

#endif

#if !defined(TEST_ROTARY_ENC) && !defined(TEST_JOYSTICK)
    sampleLeft += random(-256, 256);
    sampleRight += random(-256, 256);
#endif

    samplesCounter = samplesCounter++ % samplesCount;

    samplesLeft[samplesCounter] = sampleLeft;
    samplesRight[samplesCounter] = sampleRight;

    for (uint8_t i = 0; i < sizeof(visualisers) / sizeof(Visualiser *); i++)
    {
        visualisers[i]->draw(sampleLeft, sampleRight);
    }

    delay(50);
}
