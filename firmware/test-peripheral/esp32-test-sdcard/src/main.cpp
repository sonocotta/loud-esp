#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#define TESTONCE
#define TESTQUICK

#ifdef TFT_ENABLED
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef ESP32
#define SDCLASS SDFS
#endif
#ifdef ESP8266
#define SDCLASS SDClass
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

#ifdef TFT_ENABLED
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

void listDir(SDCLASS &fs, const char *dirname, uint8_t levels)
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

void readFile(SDCLASS &fs, const char *path)
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

  testText();
#endif

  delay(1000);

  // pinMode(PIN_SD_CS, INPUT_PULLUP);
  // pinMode(MISO, INPUT_PULLUP);
}

void loop(void)
{
#ifdef TFT_ENABLED
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
#endif

  if (!SD.begin(PIN_SD_CS)) {
    SAYLN("Card Mount Failed");
#ifdef TESTONCE
    SAYLN("Test finished");
    while (1)
      yield();
#else
    return;
#endif
  }

#ifdef ESP32
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    SAYLN("No SD card attached");
    return;
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

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

#endif

#ifdef TESTQUICK
  listDir(SD, "/", 2);

  delay(1000);
  uint32_t start_time = millis();
  readFile(SD, "/foo.txt");

  SAY("\nFinished in (ms): ");
  SAYLN(millis() - start_time);
#else

  const char *file_hello = "/hello.txt";

  Serial.printf("Opening file %s for writing\n", file_hello);
  File f = SD.open(file_hello, "w");
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
  Serial.printf("File written, %d bytes stored in %d ms\n", bytesWritten, (int)(millis() - startTime));

  // Open renamed file for reading
  Serial.printf("Reading file %s\n", file_hello);
  f = SD.open(file_hello, "r");
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

  Serial.printf("Read from file, %d bytes in %d ms\n", bytesRead, (int)(millis() - startTime));
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