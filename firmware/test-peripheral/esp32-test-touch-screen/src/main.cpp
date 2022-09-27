#include <Arduino.h>

#ifdef TEST_TFT
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef TEST_TFT_TOUCH_IRQ
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(PIN_TFT_TOUCH, PIN_TFT_TOUCH_IRQ);
#else
#ifdef PIN_TFT_TOUCH
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(PIN_TFT_TOUCH);
#endif

// Calibration values
#define TOUCH_MIN_X 236
#define TOUCH_MAX_X 3927
#define TOUCH_MIN_Y 394
#define TOUCH_MAX_Y 3706

#endif

uint32_t currentColor = 0xF000;

#define COLORS_COUNT 8
#define CELL_SIZE (TFT_WIDTH / COLORS_COUNT)
uint16_t colors[COLORS_COUNT] = {TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_BLUE, TFT_PURPLE, TFT_BLACK};

void drawButtons()
{
  for (uint8_t i = 0; i < COLORS_COUNT; i++)
  {
    tft.fillCircle(CELL_SIZE / 2, CELL_SIZE / 2 + CELL_SIZE * i, CELL_SIZE / 2, colors[i]);
  }
  tft.setCursor(1, tft.height() - CELL_SIZE / 2 - 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.println("Clear");
}

void setup()
{
  Serial.begin(SERIAL_BAUD);

#ifdef TEST_TFT
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_WHITE);
  drawButtons();
#endif

#ifdef TEST_TFT_TOUCH
  ts.begin();
  ts.setRotation(TFT_ROTATION);
#endif
}

void buttonPressed(int i)
{
  Serial.printf("Button #%d pressed\n", i);
  drawButtons();
  if (i < 7)
  {
    tft.drawCircle(CELL_SIZE / 2, CELL_SIZE / 2 + CELL_SIZE * i, CELL_SIZE / 2, TFT_BLACK);
    currentColor = colors[i];
  }
  else
  {
    tft.fillRect(CELL_SIZE, 0, tft.width(), tft.height(), TFT_WHITE);
  }
}

int lastX = -1;
int lastY = -1;

// int min_x = 500;
// int max_x = 0;
// int min_y = 500;
// int max_y = 0;

void loop()
{
  int x = 0;
  int y = 0;

  bool istouched = ts.tirqTouched() && ts.touched();
  if (istouched)
  {
    TS_Point p = ts.getPoint();

    x = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
    y = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());

    // Serial.printf("TFT Touched: %d, %d\n", x, y);
    // if (x < min_x)
    //   min_x = x;
    // if (x > max_x)
    //   max_x = x;
    // if (y < min_y)
    //   min_y = y;
    // if (y > max_y)
    //   max_y = y;

    if (lastX == -1)
    {
      if (x < CELL_SIZE)
        buttonPressed(y / CELL_SIZE);
    }
    else
      tft.drawLine(lastX, lastY, x, y, currentColor);
    lastX = x;
    lastY = y;
  }
  else
  {
    lastX = -1;
    lastY = -1;
  }
}