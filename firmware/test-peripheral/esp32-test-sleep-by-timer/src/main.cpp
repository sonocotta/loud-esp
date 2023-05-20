#include <Arduino.h>

#ifdef TEST_TFT
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#define FF20 &FreeSans24pt7b
#define FF18 &FreeSans12pt7b

#define TIME_TO_SLEEP_US 3000000

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void setup()
{
  Serial.begin(SERIAL_BAUD);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

#ifdef TEST_TFT
  gpio_hold_dis((gpio_num_t)TFT_RST);

  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FF18);
#endif

  print_wakeup_reason();
}

void loop()
{
#ifdef TEST_TFT
  tft.setCursor(0, 100);
  // tft.setTextFont(1);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN);
  tft.printf("Boot #%d\n", bootCount);
  tft.printf("Starting sleep for %d s\n", TIME_TO_SLEEP_US / 1000000);
#endif

  delay(1000);

#ifdef TEST_TFT
  digitalWrite(TFT_RST, LOW); // same pin drives backlight
  gpio_hold_en((gpio_num_t)TFT_RST);
#endif
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_US);
  esp_deep_sleep_start();
}
