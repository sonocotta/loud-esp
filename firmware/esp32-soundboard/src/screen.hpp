#pragma once

#include "SPI.h"
#include "TFT_eSPI.h"
#include <XPT2046_Touchscreen.h>

// Calibration values
#define TOUCH_MIN_X 236
#define TOUCH_MAX_X 3927
#define TOUCH_MIN_Y 394
#define TOUCH_MAX_Y 3706

#define NEXT_TOUCH_TRIG_MS 200

#include "filesystem.hpp"
#include "player.hpp"

#define IMAGE_WIDTH (tft.width() / BRD_COLS)
#define IMAGE_HEIGHT (tft.height() / BRD_ROWS)

#define TILES_CNT (BRD_ROWS * BRD_COLS)

int stateColors[] = {
    TFT_LIGHTGREY, // Init
    TFT_DARKGREEN, // Play
    TFT_YELLOW,    // Pause
    TFT_DARKGREY,  // Stop
};

class Screen
{
private:
    XPT2046_Touchscreen ts = XPT2046_Touchscreen(PIN_TFT_TOUCH);
    TFT_eSPI tft = TFT_eSPI();

    FileSystem *fs;
    Player *player;

    State savedStates[TILES_CNT] = {INIT};

public:
    Screen(FileSystem *fs, Player *player)
        : fs(fs), player(player){};

    void begin();
    void loop();
};

void Screen::begin()
{
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_WHITE);

    ts.begin();
    ts.setRotation(TFT_ROTATION);

    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(TFT_BLACK);

    for (uint8_t row = 0; row < BRD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BRD_COLS; col++)
        {
            uint8_t ix = col + row * BRD_COLS;
            auto pkg = fs->getFileByIndex(ix);
            uint16_t x = col * IMAGE_WIDTH;
            uint16_t y = row * IMAGE_HEIGHT;
            log_i("Drawing at [%d, %d] %s", x, y, pkg.image.c_str());
            tft.drawBmpFile(SD, pkg.image.c_str(), x, y);
            tft.drawString(pkg.name, x + IMAGE_WIDTH / 2, y);
            tft.drawRect(x, y, TFT_WIDTH, TFT_HEIGHT, TFT_LIGHTGREY);
        }
    }
}

void Screen::loop()
{
    static int last_x = -1, tx = 0;
    static int last_y = -1, ty = 0;
    static uint32_t last_touched_at = 0;

    if (ts.touched())
    {
        TS_Point p = ts.getPoint();
        if (p.x == 0 && p.y == 0)
            return;

        log_v("tch: %d, %d", p.x, p.y);

        tx = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, BRD_COLS);
        if (tx >= BRD_COLS)
            tx = BRD_COLS - 1;
        ty = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, BRD_ROWS);
        if (ty >= BRD_ROWS)
            ty = BRD_ROWS - 1;

        uint8_t ix = tx + ty * BRD_COLS;

        if ((tx != last_x) || (ty != last_y) || (millis() - last_touched_at > NEXT_TOUCH_TRIG_MS))
        {
            log_d("TFT Touched: [%d, %d] -> (%d, %d) -> ix = %d;", p.x, p.y, tx, ty, ix);
            player->toggle(ix);
        }

        last_x = tx;
        last_y = ty;
        last_touched_at = millis();
    }

    for (uint8_t row = 0; row < BRD_ROWS; row++)
    {
        for (uint8_t col = 0; col < BRD_COLS; col++)
        {
            uint8_t ix = col + row * BRD_COLS;
            if (savedStates[ix] != player->states[ix])
            {
                uint16_t x = col * IMAGE_WIDTH;
                uint16_t y = row * IMAGE_HEIGHT;
                tft.drawRect(x, y, IMAGE_WIDTH, IMAGE_HEIGHT, stateColors[player->states[ix]]);
                savedStates[ix] = player->states[ix];
            }
        }
    }
}