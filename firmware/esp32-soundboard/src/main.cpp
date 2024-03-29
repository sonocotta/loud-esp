#include <Arduino.h>
#include <Ticker.h>

Ticker tick;

#include "filesystem.hpp"
FileSystem filesystem;

#include "player.hpp"
Player player(&filesystem);

#include "screen.hpp"
Screen screen(&filesystem, &player);

void __tick_callback()
{
  player.debug();
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  log_d("Starting up...\n");

#ifdef PIN_TFT_TOUCH
  // when touch is connected but not used, drive CS high to disable it
  pinMode(PIN_TFT_TOUCH, OUTPUT);
  digitalWrite(PIN_TFT_TOUCH, HIGH);
#endif

  if (!filesystem.begin())
  {
    log_e("Failed to mount filesystem");
    while (1)
      ;
  }
  else
  {
    log_d("Filesystem mounted");
  }

  filesystem.loadFilesList();

  screen.begin();
  player.begin();

  // for debug purposes
  // tick.attach_ms(1000, __tick_callback);
}

void loop()
{
  screen.loop();
  player.loop();
}