#include <Arduino.h>

#include <AudioOutputI2S.h>
AudioOutputI2S *out;

#include <AudioGeneratorWAV.h>
#include <AudioGeneratorOpus.h>
#include <AudioGeneratorMP3.h>

#include "filesystem.hpp"
FileSystem filesystem;

#define TIME_REPEAT_AFTER 3000

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
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("Starting up...\n"));

  if (!filesystem.begin())
  {
    Serial.println(F("Failed to mount filesystem"));
    while (1)
      ;
  }
  else
  {
    Serial.println(F("Filesystem mounted"));
  }

  filesystem.loadFilesList();

  out = new AudioOutputI2S();
}

void setState(State newstate)
{
  Serial.print(F("State -> "));
  Serial.println(newstate);

  state_changed_at = millis();
  state = newstate;
}

void loop()
{
  static FilePackage file;

  switch (state)
  {
  case ERROR:
    // // if (file)
    // //   free(file);
    // // if (gen)
    // //   free(gen);
    // //   setState(IDLE);
    // break;

  case WAIT:
    if (millis() < state_changed_at + TIME_REPEAT_AFTER)
      break;

  case IDLE:
    file = filesystem.getFileByRandom();
    Serial.printf("Opening %s\n", file.fileName.c_str());

    if (file.generator->begin(file.file, out))
      setState(PLAY);
    else
    {
      Serial.println(F("Failed to start playback"));
      setState(ERROR);
    }
    break;

  case PLAY:
    if (file.generator->isRunning())
    {
      if (!file.generator->loop())
      {
        file.generator->stop();
        setState(WAIT);
      }
    }
    break;
  }
}