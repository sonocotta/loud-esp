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

#ifdef PIN_BATT_CHG
bool getChargingState() {
  return !digitalRead(PIN_BATT_CHG);
}
#endif

float getBatteryVoltage() {
  uint16_t bat_value = analogRead(PIN_BATT_IN);
  // Serial.printf("BATT reads %d\n", bat_value);
  float voltage = bat_value * ADC_K / 1000.0;
  return voltage;
}

float getBatteryCapacity() {
  float voltage = getBatteryVoltage();
  float capacity = (voltage - 3.3) / (4.2 - 3.3) * 100.0;
  capacity = constrain(capacity, 0, 100);
  return capacity;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
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

void loop() {
  float batteryCapacity = getBatteryCapacity();
  float batteryVoltage = getBatteryVoltage();
  
  String batteryCapacityString = String(batteryCapacity) + String("%");
  String batteryVoltageString = String(batteryVoltage) + String("V");
  Serial.print("Battery Capacitive: ");
  Serial.println(batteryCapacity);
  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);
  Serial.println();
#ifdef TEST_TFT
  int width = batteryCapacity * 234.0 / 100.0;

  tft.fillRoundRect(43, 73, 234, 94, 6, TFT_BLACK);
  tft.fillRoundRect(43, 73, width, 94, 6, TFT_GREEN);
  tft.setFreeFont(FF20);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(tft.width() /4, tft.height() /2 + 10);
  tft.println(batteryCapacityString);

  #ifdef PIN_BATT_CHG
  tft.fillRect(100, 190, 120, 20, TFT_RED);
  tft.setFreeFont(FF18);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString(batteryVoltageString, 160, 200, 2);
  tft.fillRect(100, 210, 120, 20, TFT_RED);
  
  bool isCharge = getChargingState();
  if (isCharge) {
    Serial.println("Charging");
    tft.setFreeFont(FF18);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(120, 220);
    tft.println("Charging");
  }
  #endif
#endif
  delay(1000);
}
