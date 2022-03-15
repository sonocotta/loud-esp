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

#define TIME_REPEAT_AFTER 3000

AudioOutputI2S *out;
AudioFileSourceSPIFFS *file;
AudioGeneratorWAV *wav;

uint8_t audio_files_length = 0;
char **audio_files;

enum State
{
  IDLE,
  PLAY,
  WAIT,
  ERROR
};

State state = IDLE;
uint32_t state_changed_at = 0;

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

#ifdef ESP8266
  Dir dir = SPIFFS.openDir("/");
#endif
#ifdef ESP32
  File dir = SPIFFS.open("/");
#endif

  uint8_t files_counter = 0;
#ifdef ESP8266
  while (dir.next())
#endif
#ifdef ESP32
    File file;
    while (file = dir.openNextFile())
#endif
      files_counter++;

  audio_files_length = files_counter;
  Serial.print(audio_files_length);
  Serial.println(F(" files found"));
  audio_files = (char **)malloc(audio_files_length * sizeof(char *));

  String str;
#ifdef ESP8266
  dir = SPIFFS.openDir("/");
#endif
#ifdef ESP32
  dir = SPIFFS.open("/");
#endif
  files_counter = 0;
#ifdef ESP8266
  while (dir.next())
#endif
#ifdef ESP32
    while (file = dir.openNextFile())
#endif
    {
      #ifdef ESP8266
      String fileName = dir.fileName();
      #endif
      #ifdef ESP32
      String fileName = file.name();
      #endif
      audio_files[files_counter] = (char *)malloc(fileName.length() * sizeof(char));
      strcpy(audio_files[files_counter], fileName.c_str());
      files_counter++;
    }

  for (files_counter = 0; files_counter < audio_files_length; files_counter++)
    Serial.println(audio_files[files_counter]);

  Serial.println();

  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();
}

void setState(State newstate)
{
  Serial.print(F("State -> "));
  Serial.println(newstate);

  state_changed_at = millis();
  state = newstate;
}

AudioFileSourceSPIFFS *getFileByIndex(uint8_t index)
{
  const char *fileName = audio_files[index % audio_files_length];
  Serial.println(fileName);
  return new AudioFileSourceSPIFFS(fileName);
}

void loop()
{
  switch (state)
  {

  case ERROR:
  case WAIT:
    if (millis() < state_changed_at + TIME_REPEAT_AFTER)
      break;

  case IDLE:
    file = getFileByIndex(millis());
    if (wav->begin(file, out))
      setState(PLAY);
    else
    {
      Serial.println(F("Failed to start playback"));
      setState(ERROR);
    }
    break;

  case PLAY:
    if (wav->isRunning())
    {
      if (!wav->loop())
      {
        wav->stop();
        free(file);
        setState(WAIT);
      }
    }
    break;
  }
}