#include "Visualiser.h"

#include "images/analog_black.h"
#include "images/analog_white.h"
#include "images/analog_warm.h"

#include <TFT_eSPI.h>
#include "globals.h"

// samplebuffers
extern int16_t samplesLeft[];
extern int16_t samplesRight[];

// A700Frame
bool ANALOG_ShowA700Frame = false;

// offset
uint16_t ANALOG_Offset = 0;

// mode
int16_t ANALOG_Mode = 2;

// ballistics
uint16_t ANALOG_NeedleAttack = 10;
uint16_t ANALOG_NeedleRelease = 10;
int16_t ANALOG_PeakLEDdB = 6;
uint16_t ANALOG_PeakLEDHoldTime = 500;

// peakAttenuation
float ANALOG_PeakAttenuation = 0.0f;

// segments
uint16_t ANALOG_PeakLEDColor = 0xF800;
uint16_t ANALOG_NeedleColor = 0xF81F;  

/// <summary>
/// analogmeter menu
/// </summary>
const char* strMenuANALOG[7] = {
   "Show Peak-LED from 0dB",
   "Show Peak-LED from 1dB",
   "Show Peak-LED from 2dB",
   "Show Peak-LED from 3dB",
   "Show Peak-LED from 4dB",
   "Show Peak-LED from 5dB",
   "Show Peak-LED from 6dB"
};

class AnalogVisualiser : public Visualiser
{
public:
    AnalogVisualiser(TFT_eSPI *tft) : Visualiser(tft){};

    ~AnalogVisualiser()
    {
        // tft->endWrite();
    }

    void init(void) override;
    void draw(int16_t valueLeft, int16_t valueRight) override;

private:
    uint16_t peakHoldMax = 0;
    uint16_t rmsHoldMax = 0;

    void drawNeedleLeft(int16_t value);
    void drawNeedleRight(int16_t value);
};

void AnalogVisualiser::init(void)
{
    TFT_eSPI bg = *tft;

    int cnt7 = millis();

    dBPPML = -100;
    dBPPMR = -100;

    // set rotation
    // (flipScreen) ? tft.setRotation(2) : tft.setRotation(0);
    // tft.setScroll((flipScreen) ? 320 - ANALOG_Offset : ANALOG_Offset);

    // get color from middle of the backgound picture
    uint16_t bgColor = getBackgroundColor(ANALOG_Mode);

    // fill display with black, white or warm color
    bg.fillScreen((ANALOG_ShowA700Frame) ? TFT_BLACK : bgColor);

    if (ANALOG_Mode == 0)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_black);
    if (ANALOG_Mode == 1)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_white);
    if (ANALOG_Mode == 2)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_warm);

    // A700 frame
    if (ANALOG_ShowA700Frame)
    {
        bg.fillRect(0, 0, 240, 26 + 7, TFT_BLACK);
        bg.fillRect(0, 320 - 7, 240, 7, TFT_BLACK);
        bg.fillRoundRect(0, 26, 240, 294, 7, bgColor);
    }

    // show the calibration frame
    if (showCalibrationFrame)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_MAGENTA);

    // send Calibration data
    if (calibrate)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_RED);

    // // copy background to front
    // memcpy(frontBuffer, backBuffer, 153600);

    // initialize Needles
    // draw();

    cnt7 = millis();
    Serial.printf("time passed:%d\n", cnt7 - millis());
}

void AnalogVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    TFT_eSPI bg = *tft; 
    // copy background from line 54 to 234 and bring to front
    if (ANALOG_Mode == 0)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_black);
    if (ANALOG_Mode == 1)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_white);
    if (ANALOG_Mode == 2)
        bg.pushImage(0, 0, bg.width(), bg.height(), analog_warm);

    // while(1);
    drawNeedleLeft(valueLeft);
    drawNeedleRight(valueRight);
}

/// <summary>
/// draw left Side
/// </summary>
/// <param name="init"></param>
void AnalogVisualiser::drawNeedleLeft(int16_t value)
{
    TFT_eSPI im = *tft;
    
    static uint32_t peakLEDTime = 0;
    static bool peakLEDTrigger = false;
    static float pastData = 0.0f;

    // handle peak LED
    if (peakLEDTime <= 500 && peakLEDTrigger)
    {
        // im.drawSpot(iVec2(20, 153 + 19), 7.0f, ANALOG_PeakLEDColor, 1.0f);
        im.fillCircle(20, 153 + 19, 7.0f, ANALOG_PeakLEDColor);
    }
    else
    {
        peakLEDTrigger = false;
    }

    // init?
    if ((pastData < 1.0f || pastData > 2.15f))
        pastData = 1.1f;

    // if RMS & PPM available or init
    if (value /*rms1.available() || init*/)
    {
        // read RMS, convert to dB, convert dB to radians (rotate arc for left side)
        float rmsValue = rmsAmplitudeTodB(value/*rms1.read()*/, 0);
        float peakValue = peakAmplitudeTodB(value/*peak1.read()*/, 0, ANALOG_PeakAttenuation);
        
        float arc = PI - dBToRAD(rmsValue, -40.0f, 7.0f);

        // peak LED ON
        if (peakValue >= (float)ANALOG_PeakLEDdB)
        {
            peakLEDTrigger = true;
            peakLEDTime = 0;
        }

        // ballistic
        float pastArc = pastData;
        (pastData - arc < 0) ? arc = pastArc + (ANALOG_NeedleAttack / 1000.0f) : arc = pastArc - (ANALOG_NeedleRelease / 1000.0f);
        pastData = arc;

        // draw needle
        if (arc != pastArc)
        {
            float x1 = -115.0f + 193.0f * sinf(arc);
            float y1 = (160.0f + 14.0f) + 193.0f * cosf(arc);
            float yEdge = (160.0f + 13.0f) + 115.0f * cosf(arc);
            im.drawWedgeLine(0.0f, yEdge, x1, y1, 3.0f, 3.0f, ANALOG_NeedleColor, 1.0f);
        }
    }
}

/// <summary>
/// draw right side
/// </summary>
/// <param name="init"></param>
void AnalogVisualiser::drawNeedleRight(int16_t value)
{
    TFT_eSPI im = *tft;

    static uint32_t peakLEDTime = 0;
    static bool peakLEDTrigger = false;
    static float pastData = 0.0f;

    // handle peak LED
    if (peakLEDTime <= 500 && peakLEDTrigger)
    {
        // im.drawSpot(iVec2(220, 153 + 19), 7.0f, ANALOG_PeakLEDColor, 1.0f);
        im.fillCircle(220, 153 + 19, 7.0f, ANALOG_PeakLEDColor);
    }
    else
    {
        peakLEDTrigger = false;
    }

    // init?
    if ((pastData < 1.0f || pastData > 2.15f))
        pastData = 2.16f;

    // if RMS & PPM available or init
    if (value/*rms2.available() || init*/)
    {
        // read RMS, convert to dB, convert dB to radians
        float rmsValue = rmsAmplitudeTodB(value/*rms1.read()*/, 0);
        float peakValue = peakAmplitudeTodB(value/*peak1.read()*/, 0, ANALOG_PeakAttenuation);
        
        float arc = dBToRAD(rmsValue, -40.0f, 7.0f);

        // peak LED ON
        if (peakValue >= (float)ANALOG_PeakLEDdB)
        {
            peakLEDTrigger = true;
            peakLEDTime = 0;
        }

        // ballistic
        float pastArc = pastData;
        (pastData - arc < 0) ? arc = pastArc + (ANALOG_NeedleRelease / 1000.0f) : arc = pastArc - (ANALOG_NeedleAttack / 1000.0f);
        pastData = arc;

        // draw needle
        if (arc != pastArc)
        {
            float x1 = 355.0f - 193.0f * sinf(arc);
            float y1 = (160.0f + 13.0f) - 193.0f * cosf(arc);
            float yEdge = (160.0f + 13.0f) - 115.0f * cosf(arc);
            im.drawWedgeLine(240.0f, yEdge, x1, y1, 3.0f, 3.0f, ANALOG_NeedleColor, 1.0f);
            
        }
    }
}