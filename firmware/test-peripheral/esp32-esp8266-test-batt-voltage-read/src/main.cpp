#include "SPI.h"
#include "TFT_eSPI.h"
#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifdef ESP8266
#define PIN_BATT_IN A0
#endif
#ifdef ESP32
#define PIN_BATT_IN 34
#endif


TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

  delay(1000);

  Serial.println("\n\nAnalog read test..");

  // pinMode(PIN_BATT_IN, INPUT);
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(TFT_ROTATION);
}

#define ADC_K (float)( (750 + 220) / 220 )

void loop(void)
{
  uint16_t bat_value = analogRead(PIN_BATT_IN);
  Serial.println(bat_value);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(4);
  tft.println(bat_value);
  tft.println();

  uint16_t voltage = bat_value * ADC_K;
  tft.setTextColor(TFT_BLUE);
  tft.print(voltage);
  tft.println(" mV");

  delay(2000);
}