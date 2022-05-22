#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif
#include <SD.h>
#ifdef ESP32
#include "SPIFFS.h"
#endif

#include <AudioOutputI2S.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>

AudioOutputI2S *out;
AudioFileSourceSPIFFS *file;
AudioGeneratorWAV *wav;

const uint8_t audio_files_length = 1;
const char **audio_files = (const char **)malloc(audio_files_length * sizeof(char *));

enum State
{
  STOP,
  WAIT,
  PLAY,
  ERR,
  END
};

State state = STOP;
int seq = 0;

void setup()
{
  WiFi.mode(WIFI_OFF);

  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Starting up...\n"));

  if (!SPIFFS.begin())
  {
    Serial.println(F("An Error has occurred while mounting SPIFFS"));
    while (1)
      ;
  }
  else
  {
    Serial.println(F("SPIFFS mounted"));
  }

#ifdef PIN_I2S_EN
  pinMode(PIN_I2S_EN, OUTPUT);
  digitalWrite(PIN_I2S_EN, HIGH);
#endif

  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();

  audio_files[0] = "/audiotest.wav";

  file = new AudioFileSourceSPIFFS(audio_files[seq % audio_files_length]);
  if (wav->begin(file, out))
  {
    Serial.print(audio_files[seq % audio_files_length]);
  }
}

void loop()
{
  if (wav->isRunning())
  {
    if (!wav->loop())
    {
      wav->stop();
      state = WAIT;
    }
  }
}