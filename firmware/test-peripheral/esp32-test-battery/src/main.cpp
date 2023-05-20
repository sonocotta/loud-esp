#include <Arduino.h>

#ifdef TEST_TFT
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef PIN_BATT_IN
// resistor devider values
#define ADC_K (float)((750 + 220) / 220)
#endif

#define FF20 &FreeSans24pt7b
#define FF18 &FreeSans12pt7b

// ADC calibration data
#define BATT_MAX_VOLTAGE 4200
#define BATT_MAX_ADC_VAL 2640
#define BATT_MIN_VOLTAGE 2660
#define BATT_MIN_ADC_VAL 1550

struct BatteryVoltage
{
  bool charging;
  uint32_t adcValue; // 12 bits, 0 - 4096
  uint32_t voltage;  // millivolts
  int32_t capacity;  // 0 -> 100%
};

#ifdef PIN_BATT_CHG
bool isChargingState()
{
  return digitalRead(PIN_BATT_CHG) == LOW;
}
#endif

BatteryVoltage getBatteryVoltage()
{
  bool charging =
#ifdef PIN_BATT_CHG
      !digitalRead(PIN_BATT_CHG);
#else
      false;
#endif
  uint32_t adcValue = analogRead(PIN_BATT_IN);
  uint32_t voltage = map(adcValue, BATT_MIN_ADC_VAL, BATT_MAX_ADC_VAL, BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE);
  uint32_t capacity = constrain(map(voltage, BATT_MIN_VOLTAGE, BATT_MAX_VOLTAGE, 0, 100), 0, 100);
  return BatteryVoltage{
      charging,
      adcValue,
      voltage,
      capacity};
}

void setup()
{
  Serial.begin(SERIAL_BAUD);

  analogSetPinAttenuation(PIN_BATT_IN, ADC_2_5db);

#ifdef TEST_TFT
  tft.init();
  tft.setRotation(TFT_ROTATION);
#ifdef PIN_BATT_CHG
  pinMode(PIN_BATT_CHG, INPUT_PULLUP);
#endif
  tft.fillScreen(TFT_BLACK);
  tft.drawRoundRect(40, 70, 240, 100, 10, TFT_WHITE);
  tft.drawRoundRect(270, 100, 20, 40, 4, TFT_WHITE);
  tft.fillRect(270, 100, 9, 40, TFT_BLACK); // cover the left side of the battery button
#endif
}

BatteryVoltage voltage;

void loop()
{
#ifdef TEST_TFT
  tft.setCursor(0, 0);
  tft.setTextFont(1);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK);
  tft.printf("V = %d mV, ADC = %d%", voltage.voltage, voltage.adcValue);
#endif

  voltage = getBatteryVoltage();
  Serial.printf("Charging     : %d\n", voltage.charging);
  Serial.printf("ADC reading  : %d\n", voltage.adcValue);
  Serial.printf("Batt voltage : %d mV\n", voltage.voltage);
  Serial.printf("Batt capacity: %d %\n\n", voltage.capacity);

#ifdef TEST_TFT
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_LIGHTGREY);
  tft.printf("V = %d mV, ADC = %d%", voltage.voltage, voltage.adcValue);

  int width = voltage.capacity * 234.0 / 100.0;
  tft.fillRoundRect(43, 73, 234, 94, 6, TFT_BLACK);
  tft.fillRoundRect(43, 73, width, 94, 6, TFT_GREEN);
  tft.setFreeFont(FF20);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(tft.width() / 4, tft.height() / 2 + 10);
  tft.printf("%d %%\n", voltage.capacity);

#ifdef PIN_BATT_CHG
  tft.setFreeFont(FF18);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(isChargingState() ? TFT_GREEN : TFT_BLACK);
  tft.setCursor(120, 220);
  tft.println("Charging");
#endif
#endif

  delay(1000);
}
