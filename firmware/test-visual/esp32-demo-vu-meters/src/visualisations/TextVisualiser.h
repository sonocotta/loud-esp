#include "Visualiser.h"

#include <TFT_eSPI.h>

class TextVisualiser : public Visualiser
{
public:
    TextVisualiser(TFT_eSPI* tft) : Visualiser(tft)
    {
        tft->setTextDatum(MC_DATUM);
    };
    void draw(int16_t valueLeft, int16_t valueRight) override;

private:
    uint32_t old_value;
};

void TextVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    tft->setTextColor(TFT_BLACK, TFT_BLACK);
    tft->drawNumber(old_value, tft->width() / 2, tft->height() / 2, 1);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->drawNumber(valueLeft, tft->width() / 2, tft->height() / 2, 1);
    old_value = valueLeft;
}