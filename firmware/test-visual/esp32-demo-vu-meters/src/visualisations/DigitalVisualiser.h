#include "Visualiser.h"

#include <TFT_eSPI.h>
#include "globals.h"

// samplebuffers
extern int16_t samplesLeft[];
extern int16_t samplesRight[];

bool DIGITAL_UseWheel = false;
bool DIGITAL_ShowHeader = true;
bool DIGITAL_ShowPeakBars = true;
bool DIGITAL_MenuActive = false;

// digital
uint16_t DIGITAL_SampleBlocks = 8;
uint16_t DIGITAL_dBnullPosition = 0;
bool DIGITAL_PeakHold = false;

// dB
float DIGITAL_dBLow = -40.0f;

// mode
int16_t DIGITAL_WorkMode = 0;

// offset
uint16_t DIGITAL_Offset = 0;

// A700Frame
bool DIGITAL_ShowA700Frame = false;

// colors
uint16_t DIGITAL_DisplayBackColor = 0;                    // Black
uint16_t DIGITAL_LRColor = 0xF7BE;                        // WhiteSmoke
uint16_t DIGITAL_HeaderForeColor = 0xAD55;                // DarkGray
uint16_t DIGITAL_ScaleForeColor = 0xBDF7;                 // Gray
uint16_t DIGITAL_ScaleMarkerColor = 0xBDF7;               // Gray
uint16_t DIGITAL_LineColor = 0x2945;                      // 40,40,40
uint16_t DIGITAL_VUNullDBMarkerColor = 0xB104;            // FireBrick
uint16_t DIGITAL_VUPeakColor = 0xDEFB;                    // Gainsboro
uint16_t DIGITAL_VUBarColorNormal = 0x1C9F;               // DodgerBlue
uint16_t DIGITAL_VUBarColorMid = 0xFD20;                  // Orange
uint16_t DIGITAL_VUBarColorOver = 0xF800;                 // Red
uint16_t DIGITAL_PEAKPeakColor = 0xFEA0;                  // Gold
uint16_t DIGITAL_PEAKBarColor = 0xBDF7;                   // Gray
uint16_t DIGITAL_CorrelationTextColor = 0xFEA0;           // Gold
uint16_t DIGITAL_CorrelationMinusColor = 0xF800;          // Red
uint16_t DIGITAL_CorrelationNullColor = 0xF800;           // Red
uint16_t DIGITAL_CorrelationPlusColor = 0x3666;           // LimeGreen
uint16_t DIGITAL_CorrelationBackgroundColor = 0x2104;     // 32,32,32
uint16_t DIGITAL_CorrelationMarkerColor = 0x6B4D;         // DimGray
uint16_t DIGITAL_CorrelationHoldColor = 0x6B4D;           // DimGray
uint16_t DIGITAL_CorrelationHoldBackgroundColor = 0x2104; // 32,32,32
uint16_t DIGITAL_LineMarkerColor = 0x4208;                // 64,64,64

// ballistics
uint16_t DIGITAL_VUBarAttack = 4;
uint16_t DIGITAL_VUBarRelease = 2;
uint16_t DIGITAL_VUPeakHoldTime = 50;
uint16_t DIGITAL_VUPeakRelease = 2;
uint16_t DIGITAL_PEAKBarAttack = 18;
uint16_t DIGITAL_PEAKBarRelease = 2;
uint16_t DIGITAL_PEAKPeakHoldTime = 50;
uint16_t DIGITAL_PEAKPeakRelease = 2;

// peakAttenuation
float DIGITAL_PeakAttenuation = 0.0f;

// segments
uint16_t DIGITAL_VUPeakHeight = 1;
uint16_t DIGITAL_VUSegmentHeight = 0;
uint16_t DIGITAL_PEAKPeakHeight = 5;
uint16_t DIGITAL_PEAKSegmentHeight = 1;

/// <summary>
/// digitalmeter menu
/// </summary>
const char* strMenuDIGITAL[4] = {
 "-40dB to 6dB",
 "-30dB to 6dB",
 "-40dB to 6dB Hold",
 "-30dB to 6dB Hold"
};

class DigitalVisualiser : public Visualiser
{
public:
    DigitalVisualiser(TFT_eSPI *tft) : Visualiser(tft){};

    ~DigitalVisualiser()
    {
        // tft->endWrite();
    }

    void init(void) override;
    void draw(int16_t valueLeft, int16_t valueRight) override;

private:
    uint16_t peakHoldMax = 0;
    uint16_t rmsHoldMax = 0;

    uint16_t drawLeftBarDigital(int16_t value);
    uint16_t drawRightBarDigital(int16_t value);
    void drawCorrelationDigital(uint16_t totalLength, bool reset);
    void drawHeaderDigital();
};

void DigitalVisualiser::init(void)
{
    TFT_eSPI bg = *tft;

    // fill screen
    bg.fillScreen(DIGITAL_ShowA700Frame ? TFT_BLACK : DIGITAL_DisplayBackColor);

    // show the A700 frame
    if (DIGITAL_ShowA700Frame)
        bg.fillRoundRect(0, 26, 240, 294, 7, DIGITAL_DisplayBackColor);

    // show the calibration frame
    if (showCalibrationFrame)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_MAGENTA);

    // send calibration data
    if (calibrate)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_RED);

    // draw vertical middle line
    bg.drawFastVLine(120, 40, 257, DIGITAL_LineColor);

    // draw correlation max hold box middle marker
    bg.drawFastVLine(120, 315, 3, DIGITAL_CorrelationMarkerColor);

    float dBPre = 0.0f;
    for (uint16_t y = 0; y < 260; y++)
    {
        float dB = roundf(map((float)y, 0, 260, DIGITAL_dBLow, 6.0f));

        if (dB != dBPre)
        {
            if (dB == 6)
            {
                bg.drawString("6", 46, 300 - y /*, TFT_RED, Arial_9_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, TFT_RED);
                bg.drawFastHLine(168, 296 - y, 8, TFT_RED);
                bg.drawString("6", 189, 300 - y /*, TFT_RED, Arial_9_Bold, false*/);
            }
            if (dB == 5)
            {
                bg.drawFastHLine(67, 296 - y, 4, TFT_RED);
                bg.drawFastHLine(170, 296 - y, 4, TFT_RED);
            }
            if (dB == 4)
            {
                bg.drawFastHLine(67, 296 - y, 4, TFT_RED);
                bg.drawFastHLine(170, 296 - y, 4, TFT_RED);
            }
            if (dB == 3)
            {
                bg.drawFastHLine(67, 296 - y, 4, TFT_RED);
                bg.drawFastHLine(170, 296 - y, 4, TFT_RED);
            }
            if (dB == 2)
            {
                bg.drawFastHLine(67, 296 - y, 4, TFT_RED);
                bg.drawFastHLine(170, 296 - y, 4, TFT_RED);
            }
            if (dB == 1)
            {
                bg.drawFastHLine(67, 296 - y, 4, TFT_RED);
                bg.drawFastHLine(170, 296 - y, 4, TFT_RED);
            }
            if (dB == 0)
            {
                bg.drawString("0", 46, 300 - y /*, TFT_RED, Arial_9_Bold, false*/);
                DIGITAL_dBnullPosition = y;
                bg.drawFastHLine(65, 296 - y, 8, TFT_RED);
                bg.drawFastHLine(168, 296 - y, 8, TFT_RED);
                bg.drawString("0", 189, 300 - y /*, TFT_RED, Arial_9_Bold, false*/);
            }
            if (dB == -1)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -2)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -3)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -4)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -5)
            {
                bg.drawString("5", 46, 300 - y /*, DIGITAL_ScaleForeColor, Arial_9_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(118, 296 - y, 5, DIGITAL_LineMarkerColor);
                bg.drawFastHLine(168, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawString("5", 189, 300 - y /*, DIGITAL_ScaleForeColor, Arial_9_Bold, false*/);
            }
            if (dB == -10)
            {
                bg.drawString("10", 41, 300 - y /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(118, 296 - y, 5, DIGITAL_LineMarkerColor);
                bg.drawFastHLine(168, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawString("10", 185, 300 - y /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
            }
            if (dB == -15)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -20)
            {
                bg.drawString("20", 41, 300 - y /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(118, 296 - y, 5, DIGITAL_LineMarkerColor);
                bg.drawFastHLine(168, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawString("20", 185, 300 - y /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
            }
            if (dB == -25)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -30)
            {
                bg.drawString("30", 41, 300 - y - ((DIGITAL_dBLow == -40) ? 0 : 4) /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(118, 296 - y, 5, DIGITAL_LineMarkerColor);
                bg.drawFastHLine(168, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawString("30", 185, 300 - y - ((DIGITAL_dBLow == -40) ? 0 : 4) /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
            }
            if (dB == -35)
            {
                bg.drawFastHLine(67, 296 - y, 4, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(170, 296 - y, 4, DIGITAL_ScaleMarkerColor);
            }
            if (dB == -40)
            {
                bg.drawString("40", 41, 300 - y - 4 /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
                bg.drawFastHLine(65, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawFastHLine(118, 296 - y, 5, DIGITAL_LineMarkerColor);
                bg.drawFastHLine(168, 296 - y, 8, DIGITAL_ScaleMarkerColor);
                bg.drawString("40", 185, 300 - y - 4 /*, DIGITAL_ScaleForeColor, Arial_10_Bold, false*/);
            }
            dBPre = dB;
        }
    }

    // draw correlation backgrounds
    bg.fillRect(40, 304, 161, 8, DIGITAL_CorrelationBackgroundColor);
    bg.drawFastHLine(40, 316, 161, DIGITAL_CorrelationHoldBackgroundColor);

    // draw correlation frame marker
    bg.drawFastVLine(39, 304, 8, DIGITAL_CorrelationMarkerColor);
    bg.drawFastVLine(201, 304, 8, DIGITAL_CorrelationMarkerColor);
    bg.drawFastVLine(39, 315, 3, DIGITAL_CorrelationMarkerColor);
    bg.drawFastVLine(120, 315, 3, DIGITAL_CorrelationTextColor);
    bg.drawFastVLine(201, 315, 3, DIGITAL_CorrelationMarkerColor);

    // draw correlation -1/+1
    bg.drawString("-1", 18, 316 /*, DIGITAL_CorrelationTextColor, Arial_10, false*/);
    bg.drawString("+1", 210, 316 /*, DIGITAL_CorrelationTextColor, Arial_10, false*/);

    // draw "L" & "R"
    bg.drawString("L", 11, 182 /*, DIGITAL_LRColor, Arial_18, false*/);
    bg.drawString("R", 211, 182 /*, DIGITAL_LRColor, Arial_18, false*/);
    // }

    // copy background to front
    // memcpy(frontBuffer, backBuffer, 153600);
    // draw(0);
    // tft.update(frontBuffer, true);
}

void DigitalVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    uint16_t leftBar = drawLeftBarDigital(valueLeft);
    uint16_t rightBar = drawRightBarDigital(valueRight); 
    drawCorrelationDigital(leftBar + rightBar, false);
    drawHeaderDigital();
}

/// <summary>
/// draw left bar
/// </summary>
/// <returns>rms bar length</returns>
uint16_t DigitalVisualiser::drawLeftBarDigital(int16_t value)
{
    TFT_eSPI im = *tft;
    // static vars
    static uint16_t 
        pastPeak = 0, 
        pastRms = 0, 
        rmsHoldCounter = 0, 
        peakHoldCounter = 0, 
        rmsFallingCounter = 0, 
        peakFallingCounter = 0;

    if (value/*rms1.available()*/)
    {
        // read RMS & PEAK, calculate dB & scale dB range to bar length
        float rmsValue = rmsAmplitudeTodB(value/*rms1.read()*/, 0);
        float peakValue = peakAmplitudeTodB(value/*peak1.read()*/, 0, DIGITAL_PeakAttenuation);
        
        uint16_t rmsVal = map(rmsValue, DIGITAL_dBLow, dBHigh, 0, 260);
        uint16_t peakVal = (DIGITAL_ShowPeakBars) ? map(peakValue, DIGITAL_dBLow, dBHigh, 0, 260) : 0;
        
        // get previous bar data
        uint16_t peakTmp = pastPeak;
        uint16_t rmsTmp = pastRms;

        // ballistics
        if (peakVal < peakTmp - DIGITAL_PEAKBarRelease)
            peakVal = peakTmp - DIGITAL_PEAKBarRelease; // peak
        if (peakTmp < peakVal - DIGITAL_PEAKBarAttack)
            peakVal = peakTmp + DIGITAL_PEAKBarAttack; // peak
        if (rmsVal < rmsTmp - DIGITAL_VUBarRelease)
            rmsVal = rmsTmp - DIGITAL_VUBarRelease; // rms
        if (rmsTmp < rmsVal - DIGITAL_VUBarAttack)
            rmsVal = rmsTmp + DIGITAL_VUBarAttack; // rms
        if (++rmsHoldCounter > DIGITAL_VUPeakHoldTime && rmsFallingCounter > DIGITAL_VUPeakRelease)
            rmsFallingCounter -= DIGITAL_VUPeakRelease; // rms
        if (++peakHoldCounter > DIGITAL_PEAKPeakHoldTime && peakFallingCounter > DIGITAL_PEAKPeakRelease)
            peakFallingCounter -= DIGITAL_PEAKPeakRelease; // peak

        // store bar data
        pastPeak = peakVal;
        pastRms = rmsVal;

        // store new RMS level and reset the hold counter
        if (rmsVal > rmsFallingCounter)
        {
            rmsFallingCounter = rmsVal + 1.; // +1 is very important!
            rmsHoldCounter = 0;
        }

        // store new PEAK level and reset the hold counter
        if (peakVal > peakFallingCounter)
        {
            peakFallingCounter = peakVal + 1.; // +1 is very important!
            peakHoldCounter = 0;
        }

        // store new max level for RMS peakhold
        if (rmsVal > rmsHoldMax)
        {
            rmsHoldMax = rmsVal + 1; // +1 is very important!
        }

        // store new max level for PEAK peakhold
        if (peakVal > peakHoldMax)
        {
            peakHoldMax = peakVal + 1; // +1 is very important!
        }

        // draw bars and peaks
        for (uint16_t y = 0; y < 260; y++)
        {
            float dB = roundf(map((float)y, 0, 260, DIGITAL_dBLow, 6.0f));

            uint16_t color = DIGITAL_VUBarColorNormal;
            if (DIGITAL_UseWheel)
                color = rainbow(map(y, 260, 0, 0, 127));
            else if (dB >= 0.0f)
                color = DIGITAL_VUBarColorOver;
            else if (dB >= -5.0f)
                color = DIGITAL_VUBarColorMid;

            // draw PEAK bars
            if (y < peakTmp)
            {
                im.drawFastHLine(113, 296 - y, 4, (DIGITAL_PEAKSegmentHeight == 0) ? DIGITAL_PEAKBarColor : (y % (DIGITAL_PEAKSegmentHeight + 1) == 0) ? DIGITAL_DisplayBackColor
                                                                                                                                                       : DIGITAL_PEAKBarColor); // bar
            }
            else
            {
                if (DIGITAL_PeakHold)
                {
                    im.drawFastHLine(113, 296 - y, 4, (y > 10 && y >= peakHoldMax && y < peakHoldMax + DIGITAL_PEAKPeakHeight) ? DIGITAL_PEAKPeakColor : DIGITAL_DisplayBackColor);
                }
                else
                {
                    im.drawFastHLine(113, 296 - y, 4, (y > 10 && y >= peakFallingCounter && y < peakFallingCounter + DIGITAL_PEAKPeakHeight) ? DIGITAL_PEAKPeakColor : DIGITAL_DisplayBackColor);
                }
            }

            // draw RMS bars
            if (y < rmsTmp)
            {
                im.drawFastHLine(79, 296 - y, 29, (DIGITAL_VUSegmentHeight == 0) ? color : (y % (DIGITAL_VUSegmentHeight + 1) == 0) ? DIGITAL_DisplayBackColor
                                                                                                                                    : color); // bar
            }
            else
            {
                if (y == DIGITAL_dBnullPosition)
                { // draw 0dB line
                    im.drawFastHLine(79, 296 - y, 29, DIGITAL_VUNullDBMarkerColor);
                }
                else
                { // draw peak
                    if (DIGITAL_PeakHold)
                    {
                        im.drawFastHLine(79, 296 - y, 29, (y > 10 && y >= rmsHoldMax && y <= rmsHoldMax + DIGITAL_VUPeakHeight) ? DIGITAL_VUPeakColor : DIGITAL_DisplayBackColor);
                    }
                    else
                    {
                        im.drawFastHLine(79, 296 - y, 29, (y > 10 && y >= rmsFallingCounter && y < rmsFallingCounter + DIGITAL_VUPeakHeight) ? DIGITAL_VUPeakColor : DIGITAL_DisplayBackColor);
                    }
                }
            }
        }
        return rmsVal;
    }
    return 0;
}

/// <summary>
/// draw right bar
/// </summary>
/// <returns>rms bar length</returns>
uint16_t DigitalVisualiser::drawRightBarDigital(int16_t value)
{
    TFT_eSPI im = *tft;
    // static vars
    static uint16_t pastPeak = 0, 
        pastRms = 0, 
        rmsHoldCounter = 0, 
        peakHoldCounter = 0, 
        rmsFallingCounter = 0, 
        peakFallingCounter = 0;

    if (value/*rms2.available()*/)
    {
        // read RMS & PEAK, calculate dB & scale dB range to bar length
        uint16_t rmsVal = map(rmsAmplitudeTodB(value/*rms2.read()*/, 1), DIGITAL_dBLow, dBHigh, 0, 260);
        uint16_t peakVal = (DIGITAL_ShowPeakBars) ? map(peakAmplitudeTodB(value/*peak2.read()*/, 1, DIGITAL_PeakAttenuation), DIGITAL_dBLow, dBHigh, 0, 260) : 0;

        // get previous bar data
        uint16_t peakTmp = pastPeak;
        uint16_t rmsTmp = pastRms;

        // ballistics
        if (peakVal < peakTmp - DIGITAL_PEAKBarRelease)
            peakVal = peakTmp - DIGITAL_PEAKBarRelease; // peak
        if (peakTmp < peakVal - DIGITAL_PEAKBarAttack)
            peakVal = peakTmp + DIGITAL_PEAKBarAttack; // peak
        if (rmsVal < rmsTmp - DIGITAL_VUBarRelease)
            rmsVal = rmsTmp - DIGITAL_VUBarRelease; // rms
        if (rmsTmp < rmsVal - DIGITAL_VUBarAttack)
            rmsVal = rmsTmp + DIGITAL_VUBarAttack; // rms
        if (++rmsHoldCounter > DIGITAL_VUPeakHoldTime && rmsFallingCounter > DIGITAL_VUPeakRelease)
            rmsFallingCounter -= DIGITAL_VUPeakRelease; // rms
        if (++peakHoldCounter > DIGITAL_PEAKPeakHoldTime && peakFallingCounter > DIGITAL_PEAKPeakRelease)
            peakFallingCounter -= DIGITAL_PEAKPeakRelease; // peak

        // store bar data
        pastPeak = peakVal;
        pastRms = rmsVal;

        // store new RMS level and reset the hold counter
        if (rmsVal > rmsFallingCounter)
        {
            rmsFallingCounter = rmsVal + 1.; // +1 is very important!
            rmsHoldCounter = 0;
        }

        // store new PEAK level and reset the hold counter
        if (peakVal > peakFallingCounter)
        {
            peakFallingCounter = peakVal + 1.; // +1 is very important!
            peakHoldCounter = 0;
        }

        // store new max level for RMS peakhold
        if (rmsVal > rmsHoldMax)
        {
            rmsHoldMax = rmsVal + 1; // +1 is very important!
        }

        // store new max level for PEAK peakhold
        if (peakVal > peakHoldMax)
        {
            peakHoldMax = peakVal + 1; // +1 is very important!
        }

        // draw bars and peaks
        for (uint16_t y = 0; y < 260; y++)
        {
            float dB = roundf(map((float)y, 0, 260, DIGITAL_dBLow, 6.0f));

            uint16_t color = DIGITAL_VUBarColorNormal;
            if (DIGITAL_UseWheel)
                color = rainbow(map(y, 260, 0, 0, 127));
            else if (dB >= 0.0f)
                color = DIGITAL_VUBarColorOver;
            else if (dB >= -5.0f)
                color = DIGITAL_VUBarColorMid;

            // draw PEAK bars
            if (y < peakTmp)
            {
                im.drawFastHLine(124, 296 - y, 4, (DIGITAL_PEAKSegmentHeight == 0) ? DIGITAL_PEAKBarColor : (y % (DIGITAL_PEAKSegmentHeight + 1) == 0) ? DIGITAL_DisplayBackColor
                                                                                                                                                       : DIGITAL_PEAKBarColor); // bar
            }
            else
            {
                if (DIGITAL_PeakHold)
                {
                    im.drawFastHLine(124, 296 - y, 4, (y > 10 && y >= peakHoldMax && y < peakHoldMax + DIGITAL_PEAKPeakHeight) ? DIGITAL_PEAKPeakColor : DIGITAL_DisplayBackColor);
                }
                else
                {
                    im.drawFastHLine(124, 296 - y, 4, (y > 10 && y >= peakFallingCounter && y < peakFallingCounter + DIGITAL_PEAKPeakHeight) ? DIGITAL_PEAKPeakColor : DIGITAL_DisplayBackColor);
                }
            }

            // draw RMS bars
            if (y < rmsTmp)
            {
                im.drawFastHLine(133, 296 - y, 29, (DIGITAL_VUSegmentHeight == 0) ? color : (y % (DIGITAL_VUSegmentHeight + 1) == 0) ? DIGITAL_DisplayBackColor
                                                                                                                                     : color); // bar
            }
            else
            {
                if (y == DIGITAL_dBnullPosition)
                { // draw 0dB line
                    im.drawFastHLine(133, 296 - y, 29, DIGITAL_VUNullDBMarkerColor);
                }
                else
                { // draw peak
                    if (DIGITAL_PeakHold)
                    {
                        im.drawFastHLine(133, 296 - y, 29, (y > 10 && y >= rmsHoldMax && y <= rmsHoldMax + DIGITAL_VUPeakHeight) ? DIGITAL_VUPeakColor : DIGITAL_DisplayBackColor);
                    }
                    else
                    {
                        im.drawFastHLine(133, 296 - y, 29, (y > 10 && y >= rmsFallingCounter && y < rmsFallingCounter + DIGITAL_VUPeakHeight) ? DIGITAL_VUPeakColor : DIGITAL_DisplayBackColor);
                    }
                }
            }
        }
        return rmsVal;
    }
    return 0;
}

/// <summary>
/// draw correlation
/// </summary>
/// <param name="totalLength"></param>
/// <param name="reset"></param>
void DigitalVisualiser::drawCorrelationDigital(uint16_t totalLength, bool reset)
{
    TFT_eSPI im = *tft;
    // static vars
    static float avgPrev = 0.0f;
    static uint16_t frame = 0, negativeMax = 0, negativeMaxHold = 0, positiveMax = 0, positiveMaxHold = 0;

    // vars
    float normL = 0.0f, normR = 0.0f, value = 0.0f, runningTotal = 0.0f, avg = 0.0f;
    uint16_t corrPos = 0, corrBarLength = 0;

    // loop through the samples
    for (uint16_t i = 0; i < DIGITAL_SampleBlocks * 128; i++)
    {
        normL = samplesLeft[i] / 32767.0f;
        normR = samplesRight[i] / 32767.0f;

        // correlation
        runningTotal += cosf((atan2f(normL, normR) - 0.78539816f) * 2.0f);
    }

    // correlation avg
    avg = runningTotal / (float)(DIGITAL_SampleBlocks * 128);

    // bars avaiable -> reset clear trigger, not -> reset avg
    if (totalLength > 0)
        frame = 0;
    else
        avg = 0.0f;

    // ballistics for correlation
    value = (abs(avg - avgPrev) > 0.01f) ? (avg > avgPrev) ? avgPrev + 0.01f : avgPrev - 0.01f : avg;
    avgPrev = value;

    // delimiter
    if (value > 1.0f)
        value = 1.0f;
    if (value < -1.0f)
        value = -1.0f;

    // set correlation box color from value and hysteresis
    corrPos = 120.0f + (80.0f * value);
    corrBarLength = abs(80.0f * value);

    // set colors for correlation rectangle and store min/max values for correlation maxhold view
    uint16_t corBox = DIGITAL_CorrelationNullColor;
    if (corrPos > 120)
    {
        if (corrPos > 122)
            corBox = DIGITAL_CorrelationPlusColor;
        if (corrBarLength > positiveMax)
        {
            positiveMax = corrBarLength;
            if (positiveMax > positiveMaxHold)
                positiveMaxHold = positiveMax;
        }
    }
    else if (corrPos < 120)
    {
        if (corrPos < 118)
            corBox = DIGITAL_CorrelationMinusColor;
        if (corrBarLength > negativeMax)
        {
            negativeMax = corrBarLength;
            if (negativeMax > negativeMaxHold)
                negativeMaxHold = negativeMax;
        }
    }
    else
    {
        corBox = DIGITAL_CorrelationNullColor;
    }

    // optical delimiter
    if (corrPos <= 40)
        corrPos = 40;
    else if (corrPos > 194)
        corrPos = 194;
    else
        corrPos -= 3;

    // reset maxhold values after 2000 frames or reset
    if ((++frame == 2000 && !DIGITAL_PeakHold) || reset)
    {
        negativeMax = 0;
        positiveMax = 0;
        positiveMaxHold = 0;
        negativeMaxHold = 0;
        // // copy background to front (from line 304 to 317)
        // memcpy(frontBuffer + 72960, backBuffer + 72960, 6240);
    }
    else
    {
        // draw correlation box and middle marker
        im.fillRect(40, 304, 161, 8, DIGITAL_CorrelationBackgroundColor);
        im.drawFastVLine(120, 304, 8, DIGITAL_CorrelationTextColor);
        im.fillRect(corrPos, 304, 7, 8, corBox);
    }

    // draw negative hold correlation
    if (negativeMaxHold > 0)
        im.drawFastHLine(120 - negativeMaxHold, 316, negativeMaxHold, DIGITAL_CorrelationHoldColor);

    // draw positive hold correlation
    if (positiveMaxHold > 0)
        im.drawFastHLine(121, 316, positiveMaxHold, DIGITAL_CorrelationHoldColor);
}

/// <summary>
/// draw header
/// </summary>
void DigitalVisualiser::drawHeaderDigital()
{
    TFT_eSPI im = *tft;
    if (DIGITAL_ShowHeader)
    {
        im.fillRect(80, 32, 80, 10, DIGITAL_DisplayBackColor);
        im.drawString(DIGITAL_ShowPeakBars ? "RMS  PPM  RMS" : "RMS", DIGITAL_ShowPeakBars ? 83 : 110, 41/*, DIGITAL_HeaderForeColor, Arial_8, false*/);
    }
}