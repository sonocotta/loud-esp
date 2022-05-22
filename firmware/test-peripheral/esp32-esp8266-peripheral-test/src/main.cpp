#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#ifdef TEST_AUDIO
#include <SD.h>
#ifdef ESP32
#include "SPIFFS.h"
#endif
#include <AudioOutputI2S.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>

AudioOutputI2S *out = new AudioOutputI2S();
AudioFileSourceSPIFFS *file;
AudioGeneratorWAV *wav = new AudioGeneratorWAV();
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifdef TEST_TFT
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef TEST_BATT_IN
// resistor devider values
#define ADC_K (float)((750 + 220) / 220)
#endif

#ifdef TEST_IR
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define IR_BUF_SIZE 1024
#define IR_TIMEOUT 50
#define IR_TOLERANCE 25

IRrecv IrReciever(TEST_IR, IR_BUF_SIZE, IR_TIMEOUT, true);
decode_results dresults;
#endif

#ifdef TEST_RGB
#include <FastLED.h>
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];
#endif

#ifdef TEST_TFT_TOUCH_IRQ
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(TEST_TFT_TOUCH, TEST_TFT_TOUCH_IRQ);
#else 
#ifdef TEST_TFT_TOUCH
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(TEST_TFT_TOUCH);
#endif
#endif

#ifdef TEST_ROTARY_ENC
#include <ESP32Encoder.h>
ESP32Encoder encoder;
#endif

enum TestType
{
  AUTO,
  USER,
  MANUAL
};

struct TestDef
{
  String name;
  String result;
  bool userConfirmed;
  String userInquiry;
  bool passed;
  bool (*validate)();
};

#define RESULT_STARTS_AT 20

TestDef tests[] = {
#ifdef TEST_TFT
    {"TFT", "", true, "Can you see image on the screen?", false, NULL},
#endif
#ifdef TEST_AUDIO
    {"AUDIO", "", true, "Can you hear sound in both channels?", false, NULL},
#endif
#ifdef TEST_BATT_IN
    {"BATT UP", "", false, "", false, []()
     {
       uint16_t bat_value = analogRead(TEST_BATT_IN);
       Serial.printf("BATT reads %d\n", bat_value);
       return (bat_value > 1000) && (bat_value < 2048);
     }},
    {"BATT DN", "", false, "", false, []()
     {
       uint16_t bat_value = analogRead(TEST_BATT_IN);
       Serial.printf("BATT reads %d\n", bat_value);
       return (bat_value > 600) && (bat_value < 1000);
     }},
#endif
#ifdef TEST_IR
    {"IR IN", "", false, "", false, []()
     {
       bool decoded = false;
       dresults.decode_type = UNUSED;
       IrReciever.resume();
       delay(500);
       yield();
       if (IrReciever.decode(&dresults))
       {
         Serial.printf("IR receiver %d\n", dresults.value);
         decoded = true;
       }
       return decoded;
     }},
#endif
#ifdef TEST_SD
    {"SD CARD", "", false, "", false, []()
     {
       if (!SD.begin(TEST_SD))
       {
         Serial.println("Card Mount Failed");
         return false;
       }

       File root = SD.open("/");
       File entry = root.openNextFile();
       if (!entry)
       {
         Serial.println("Card is empty");
         return false;
       }
       Serial.printf("Found file: %s (%d bytes)\n", entry.name(), entry.size());
       return true;
     }},
#endif
#ifdef TEST_PSRAM
    {"PSRAM", "", false, "", false, []()
     {
#ifdef ESP32
       Serial.printf("PSRAM: %d\n", ESP.getFreePsram());
#endif
       char *tmp = (char *)ps_malloc(1024);
       return (tmp != 0);
     }},
#endif
#ifdef TEST_TOUCH
    {"TOUCH", "", false, "", false, []()
     {
       Serial.printf("Built-in touch: %d \n",
                     touchRead(TEST_TOUCH));
       return (touchRead(TEST_TOUCH) != 0);
     }},
#endif
#ifdef TEST_RGB
    {"RGB", "", true, "Can you see red RGB led?", false, NULL},
#endif
#ifdef TEST_TFT_TOUCH_IRQ
    {"TFT IRQ", "", false, "", false, []()
     {
       bool istouched = ts.tirqTouched() && ts.touched();
       if (istouched)
       {
         TS_Point p = ts.getPoint();
         Serial.printf("TFT Touched: %d, %d \n", p.x, p.y);
         if ((p.x != 0) || (p.y != 0))
           return true;
       }
       return false;
     }},
#else 
#ifdef TEST_TFT_TOUCH
    {"TFT TOUCH", "", false, "", false, []()
     {
       bool istouched = ts.touched();
       if (istouched)
       {
         TS_Point p = ts.getPoint();
         Serial.printf("TFT Touched: %d, %d \n", p.x, p.y);
         if ((p.x != 0) || (p.y != 0))
           return true;
       }
       return false;
     }},
#endif
#endif
#ifdef TEST_JOYSTICK
    {"JOYSTICK", "", false, "", false, []()
     {
       uint32_t x = (analogRead(TEST_JOYSTICK_A) >> 4);
       uint32_t y = (analogRead(TEST_JOYSTICK_B) >> 4);
       Serial.printf("Joystick readings: %d, %d, %d\n", y, x, digitalRead(TEST_JOYSTICK_BTN));
       return (x > 64) && (x < 256) && (y > 64) && (y < 256);
     }},
#endif
#ifdef TEST_ROTARY_ENC
    {"ENC ROTA", "", false, "", false, []()
     {
       auto encRead = encoder.getCount();
       Serial.printf("Encoder reads: %d\n", encRead);
       return (encRead > 10);
     }},
#endif
#ifdef TEST_ROTARY_ENC
    {"ENC BTN", "", false, "", false, []()
     {
       auto btnRead = digitalRead(TEST_ROTARY_ENC);
       Serial.printf("Encoder button reads: %s", btnRead == 1 ? "high" : "low");
       return btnRead == 0;
     }},
#endif
};

#define tests_count (sizeof(tests) / sizeof(*tests))

#ifdef TEST_TFT
unsigned long testText()
{
  tft.fillScreen(TFT_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  // tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}
#endif

void setup()
{
#ifdef TEST_TFT
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setViewport(0, 0, TFT_WIDTH, TFT_HEIGHT);
#ifdef TFT_ROTATION
  tft.setRotation(TFT_ROTATION);
#endif
#endif

#ifdef TEST_AUDIO
  if (!SPIFFS.begin())
  {
    Serial.begin(SERIAL_BAUD);
    while (!Serial)
      ;
    Serial.println(F("An Error has occurred while mounting SPIFFS"));
    while (1)
      ;
  }
  else
  {
    Serial.println(F("SPIFFS mounted"));
  }
#endif

#ifdef TEST_TFT
  Serial.println(F("Testing TFT ... "));
  testText();
#endif

#ifdef TEST_RGB
  FastLED.addLeds<WS2812B, TEST_RGB, GRB>(leds, NUM_LEDS);

  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
#endif

#ifdef TEST_AUDIO
  Serial.print(F("Testing audio ... "));

  file = new AudioFileSourceSPIFFS("/audiotest.wav");
  if (!wav->begin(file, out))
  {
    Serial.begin(SERIAL_BAUD);
    while (!Serial)
      ;
    Serial.println("Audio play FAILED!");
  }
  else
  {
    Serial.println("started");
  }

  bool exitAudio = false;
  while (!exitAudio)
  {
    if (wav->isRunning())
    {
      if (!wav->loop())
      {
        wav->stop();
        exitAudio = true;
      }
    }
    yield();
  }
#endif

#ifdef TEST_IR
  IrReciever.setTolerance(IR_TOLERANCE);
  IrReciever.enableIRIn();
#endif

#ifdef TEST_TFT_TOUCH
  ts.begin();
  ts.setRotation(1);
#endif

#ifdef TEST_JOYSTICK
  pinMode(TEST_JOYSTICK_BTN, INPUT_PULLUP);
#endif

#ifdef TEST_ROTARY_ENC
  pinMode(TEST_ROTARY_ENC, INPUT);
  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachHalfQuad(TEST_ROTARY_ENC_A, TEST_ROTARY_ENC_B);
#endif

  // manual tests
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;
  delay(1000);

  Serial.println("\n\nESP development boards hw test ..");

  for (uint8_t i = 0; i < tests_count; i++)
  {
    auto test = tests[i];
    if (!test.passed)
    {
      if (test.userConfirmed)
      {
        Serial.printf("[%s] %s\n", test.name, test.userInquiry.c_str());
        bool hasAnswer = false;
        while (!hasAnswer)
        {
          while (!Serial.available())
          {
            // Serial.print(".");
            delay(10);
          }
          uint8_t c = Serial.read();

          Serial.print("<-");
          Serial.println(c);

          if ((c == 89) || (c == 121))
          {
            tests[i].passed = true;
            hasAnswer = true;
          }
          else if ((c == 78) || (c == 110))
          {
            tests[i].passed = false;
            hasAnswer = true;
          }
        }
      }
    }
  }

#ifdef TEST_RGB
  leds[0] = CRGB::Black;
  FastLED.show();
#endif
}

void loop(void)
{

#ifdef TEST_TFT
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
#endif

  // validate auto tests
  for (uint8_t i = 0; i < tests_count; i++)
  {
    auto test = tests[i];
    if (!test.passed)
    {
      if (!test.userConfirmed)
      {
        Serial.printf("Starting %s\n", test.name);
        tests[i].passed = test.validate();
      }
    }
  }

  // indicate status
  for (uint8_t i = 0; i < tests_count; i++)
  {
    auto test = tests[i];

#ifdef TEST_TFT
    tft.setTextColor(TFT_WHITE);
    tft.printf("[%s] ", test.name.c_str());
    for (uint8_t x = 0; x < RESULT_STARTS_AT - test.name.length() - 2; x++)
      tft.print(' ');
    if (test.passed)
    {
      tft.setTextColor(TFT_GREEN);
      tft.println("PASS");
    }
    else
    {
      tft.setTextColor(TFT_RED);
      tft.println("FAIL");
    }
#else
    Serial.printf("[%s] ", test.name.c_str());
    for (uint8_t x = 0; x < RESULT_STARTS_AT - test.name.length() - 2; x++)
      Serial.print(' ');
    if (test.passed)
    {
      Serial.println("PASS");
    }
    else
    {
      Serial.println("FAIL");
    }

#endif
  }

#if defined(TEST_JOYSTICK) && defined(TEST_TFT)
  uint32_t x = (analogRead(TEST_JOYSTICK_A) >> 4);
  uint32_t y = (analogRead(TEST_JOYSTICK_B) >> 4);

  tft.setTextColor(TFT_BLUE);
  tft.setCursor(x, y);
  tft.print('X');
#endif

  Serial.println("\n\n");

  delay(1000);
}