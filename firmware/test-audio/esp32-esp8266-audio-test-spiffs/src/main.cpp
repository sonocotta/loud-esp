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

const uint8_t audio_files_length = 4;
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

  audio_files[0] = "/1000Hz_-3dBFS_2s.wav";
  audio_files[1] = "/2000Hz_-3dBFS_2s.wav";
  audio_files[2] = "/10000Hz_-3dBFS_2s.wav";
  audio_files[3] = "/20000Hz_-3dBFS_2s.wav";

  file = new AudioFileSourceSPIFFS(audio_files[seq % audio_files_length]);
  if (wav->begin(file, out))
  {
    Serial.print(audio_files[seq % audio_files_length]);
  }
}

void loop()
{

  switch (state)
  {
  case WAIT:
    delay(250);
    Serial.println(" <");
    state = STOP;
    break;

  case STOP:
    Serial.print("> ");
    if (file)
      free(file);
    seq++;

    if (seq > audio_files_length)
      break;

    Serial.print(audio_files[seq % audio_files_length]);
    file = new AudioFileSourceSPIFFS(audio_files[seq % audio_files_length]);

    if (wav->begin(file, out))
    {
      state = PLAY;
    }
    else
    {
      state = ERR;
    }
    break;

  case PLAY:
    if (wav->isRunning())
    {
      if (!wav->loop())
      {
        wav->stop();
        state = WAIT;
      }
    }
    break;

  case ERR:
    Serial.println("error!");
    state = END;
    break;

  case END:
    break;
  }
}