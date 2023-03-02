#include "Visualiser.h"

#include <TFT_eSPI.h>

#define COLOR_DEPTH 16
#define SPRITE_SIZE 32

class BufferedVisualiser : public Visualiser
{
public:
    BufferedVisualiser(TFT_eSPI *tft) : Visualiser(tft)
    {
    };

    ~BufferedVisualiser()
    {
        tft->endWrite();
    }

    void init(void) override;
    void draw(int16_t valueLeft, int16_t valueRight) override;
 
private:
    TFT_eSprite spr[2] = {TFT_eSprite(tft), TFT_eSprite(tft)};
    bool sprSel = 0;
    uint16_t *sprPtr[2];

    int16_t x = 0, y = 0, dx = 1, dy = 1;
};

void BufferedVisualiser::init(void)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        spr[i].setColorDepth(COLOR_DEPTH);
        sprPtr[i] = (uint16_t *)spr[i].createSprite(SPRITE_SIZE, SPRITE_SIZE);
        spr[i].setTextDatum(MC_DATUM);
    }
    tft->initDMA();
    tft->startWrite();
}

void BufferedVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    x += dx;
    y += dy;
    if ((x + SPRITE_SIZE > tft->width()) || (x < 0)) { dx *= -1; }
    if ((y + SPRITE_SIZE > tft->height()) || (y < 0)) { dy *= -1; }

    if (tft->dmaBusy())
        ;

    // draw
    spr[sprSel].fillSprite(TFT_BLACK);
    spr[sprSel].setTextColor(TFT_GREEN, TFT_BLACK);
    spr[sprSel].drawNumber(valueLeft, SPRITE_SIZE / 2, SPRITE_SIZE / 2, 1);

    // send to tft
    tft->pushImageDMA(x, y, SPRITE_SIZE, SPRITE_SIZE, sprPtr[sprSel]);

    sprSel = !sprSel;
}