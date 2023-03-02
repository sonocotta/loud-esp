#include "Visualiser.h"

#include <TFT_eSPI.h>

class SerialVisualiser : public Visualiser
{
public:
    SerialVisualiser(TFT_eSPI* tft) : Visualiser(tft){};
    void draw(int16_t valueLeft, int16_t valueRight) override;
};

void SerialVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    Serial.printf("L: %d; R: %d;\n", valueLeft, valueRight);
}