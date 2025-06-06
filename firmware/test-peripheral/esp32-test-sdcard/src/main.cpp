#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define TESTONCE
//#define TESTQUICK

#ifdef TFT_ENABLED
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef TEST_SDIO
  #include <SD_MMC.h>
  #define SD_HW SD_MMC
  #define SD_FS SDMMCFS
#endif

#ifdef PIN_SD_CS
  #define SD_HW SD
  #define SD_FS SDFS
#endif

#ifdef TFT_ENABLED
#define SAYLN(x)     \
  Serial.println(x); \
  tft.println(x);
#define SAY(x)     \
  Serial.print(x); \
  tft.print(x);
#else
#define SAYLN(x) \
  Serial.println(x);
#define SAY(x) \
  Serial.print(x);
#endif

void listDir(SD_FS &fs, const char *dirname, uint8_t levels)
{
  SAY("Listing directory: ");
  SAYLN(dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    SAYLN("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    SAYLN("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      SAY("  DIR : ");
      SAYLN(file.name());
      // #ifdef ESP32
      // if(levels){
      //     listDir(fs, file.path(), levels -1);
      // }
      // #endif
    }
    else
    {
      SAY("  FILE: ");
      SAY(file.name());
      SAY(";  SIZE: ");
      SAYLN(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(SD_FS &fs, const char *path)
{
  SAY("Reading file: ");
  SAYLN(path);

  File file = fs.open(path);
  if (!file)
  {
    SAYLN("Failed to open file for reading");
    return;
  }

  SAY("Read from file: ");
  while (file.available())
  {
    SAY(file.read());
  }
  file.close();
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

#ifdef TFT_ENABLED
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setViewport(0, 0, TFT_WIDTH, TFT_HEIGHT);
#ifdef TFT_ROTATION
  tft.setRotation(TFT_ROTATION);
#endif
#endif

  delay(1000);
}

void loop(void)
{
#ifdef TFT_ENABLED
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
#endif

#if defined(TEST_SDIO)
  pinMode(2, INPUT_PULLUP);
  if (!SD_HW.begin("/sdcard", true)) { // <- uncomment for 1-bit mode
#endif
#if defined(PIN_SD_CS)
  if (!SD_HW.begin(PIN_SD_CS)) {
#endif
      SAYLN("Card Mount Failed");
#ifdef TESTONCE
      SAYLN("Test finished");
      while (1)
        yield();
#else
  return;
#endif
    } else {
      SAYLN("Card Mount Success");
    }

#ifdef ESP32
    uint8_t cardType = SD_HW.cardType();

    if (cardType == CARD_NONE)
    {
      SAYLN("No SD card attached");
      // return;
    }

    SAY("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
      SAYLN("MMC");
    }
    else if (cardType == CARD_SD)
    {
      SAYLN("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
      SAYLN("SDHC");
    }
    else
    {
      SAYLN("UNKNOWN");
    }

    uint64_t cardSize = SD_HW.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

#endif

#ifdef TESTQUICK
    listDir(SD_HW, "/", 2);

    delay(1000);
    uint32_t start_time = millis();
    readFile(SD_HW, "/foo.txt");

    SAY("\nFinished in (ms): ");
    SAYLN(millis() - start_time);
#else

const char *file_hello = "/hello.txt";

Serial.printf("Opening file %s for writing\n", file_hello);
File f = SD_HW.open(file_hello, "w");
if (f == NULL)
{
  Serial.println("Failed to open file for writing");
  return;
}

char *text = "THETEST";
uint32_t bytesWritten = 0;
uint32_t startTime = millis();
while (bytesWritten < 1024 * 1024)
{
  f.println(text);
  // fprintf(f, "%s\n", text);
  bytesWritten += strlen(text) + 1;
  yield();
}
f.close();
int ms_spend = (int)(millis() - startTime);
Serial.printf("File written, %d bytes stored in %d ms, speed = %2.2f  kB/s\n", bytesWritten, ms_spend, (float)bytesWritten / ms_spend);

// Open renamed file for reading
Serial.printf("Reading file %s\n", file_hello);
f = SD_HW.open(file_hello, "r");
if (f == NULL)
{
  Serial.println("Failed to open file for reading");
  return;
}

// Read a line from file
uint32_t bytesRead = 0;
startTime = millis();
uint8_t line[64];
while (f.available())
{
  f.read(line, sizeof line);
  bytesRead += sizeof line;
  yield();
}

int ms_read = (int)(millis() - startTime);
Serial.printf("Read from file, %d bytes in %d ms, speed = %2.2f kB/s\n", bytesRead, ms_read, (float)bytesRead / ms_read);
f.close();
#endif

#ifdef TESTONCE
    SAYLN("Test finished");
    while (1)
      yield();
#else
SAYLN("Restart in 5 seconds ... ");
delay(5000);
#endif
}