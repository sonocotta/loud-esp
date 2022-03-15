#include <Arduino.h>
#include <SD.h>

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

  delay(1000);
}

void loop(void)
{
  delay(1000);

  if (!SD.begin(PIN_SD_CS))
  {
    // Serial.println("Card Mount Failed");
    return;
  }

  File root = SD.open("/");
  File entry = root.openNextFile();
  if (!entry)
  {
    Serial.println("Card is empty");
  }
  Serial.printf("Found file: %s (%d bytes)\n", entry.name(), entry.size()); 
}