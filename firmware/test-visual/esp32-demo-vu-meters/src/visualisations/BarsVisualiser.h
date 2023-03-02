#include "Visualiser.h"

#include <TFT_eSPI.h>
#include "globals.h"

#include "mock/FakeFFT1024.h"
FakeFFT1024 fft1024_1;
FakeFFT1024 fft1024_2 = fft1024_1;
FakeFFT1024 fft1024_3;

// samplebuffers
extern int16_t samplesLeft[];
extern int16_t samplesRight[];

bool FFT_UseWheel = true;
bool FFT_PeakHold = false;

// bins
uint16_t binStart16[16] = { 0 }, binEnd16[16] = { 0 };
uint16_t binStart25[25] = { 0 }, binEnd25[25] = { 0 };
uint16_t binStart52[52] = { 0 }, binEnd52[52] = { 0 };
uint16_t binStart69[69] = { 0 }, binEnd69[69] = { 0 };

// dB
float FFT_dBLow = -40.0f;

// A700Frame
bool FFT_ShowA700Frame = false;

// offset
uint16_t FFT_Offset = 0;

// mode
int16_t FFT_WorkMode = 1;
int16_t FFT_BarMode = 5;
int16_t FFT_LevelBarMode = 0; // 0=peak, 1=vu

// peakAttenuation
float FFT_PeakAttenuation = 0.0f;

// ballistics
uint16_t FFT_VUBarAttack = 16;
uint16_t FFT_VUBarRelease = 2;
uint16_t FFT_VUPeakHoldTime = 50;
uint16_t FFT_VUPeakRelease = 2;
uint16_t FFT_PEAKBarAttack = 16;
uint16_t FFT_PEAKBarRelease = 2;
uint16_t FFT_PEAKPeakHoldTime = 50;
uint16_t FFT_PEAKPeakRelease = 2;
uint16_t FFT_FFTBarAttack = 12;
uint16_t FFT_FFTBarRelease = 3;
uint16_t FFT_FFTPeakHoldTime = 100;
uint16_t FFT_FFTPeakRelease = 1;

// segments
uint16_t FFT_VUPeakHeight = 5;
uint16_t FFT_VUSegmentHeight = 1;
uint16_t FFT_FFTPeakHeight = 1;
uint16_t FFT_FFTSegmentHeight = 0;

uint16_t FFT_DisplayBackColor = 0;          // Black
uint16_t FFT_LRColor = 0xF7BE;              // WhiteSmoke
uint16_t FFT_LineColor = 0x4208;            // 64,64,64
uint16_t FFT_ScaleForeColor = 0xBDF7;       // Gray
uint16_t FFT_ScaleBackgroundColor = 0x10A2; // 20,20,20
uint16_t FFT_VUBarsBackColor = 0x2104;      // 32,32,32
uint16_t FFT_VUBarColorNormal = 0x1C9F;     // DodgerBlue
uint16_t FFT_VUBarColorMid = 0xFD20;        // Orange
uint16_t FFT_VUBarColorOver = 0xF800;       // Red
uint16_t FFT_VUPeakColor = 0xFEA0;          // Gold
uint16_t FFT_VUMarkerColor = 0xFF9C;        // Linen
uint16_t FFT_FFTBarColor = 0x1C9F;          // DodgerBlue
uint16_t FFT_FFTPeakColor = 0xDEFB;         // Gainsboro
uint16_t FFT_HeaderForeColor = 0xAD55;

/// <summary>
/// spectrumanalyzer menu
/// </summary>
const char *strMenuFFT[12] = {
    "-40dB to 6dB RMS",
    "-40dB to 6dB PPM",
    "-30dB to 6dB RMS",
    "-30dB to 6dB PPM",
    "-40dB to 6dB RMS Hold",
    "-40dB to 6dB PPM Hold",
    "-30dB to 6dB RMS Hold",
    "-30dB to 6dB PPM Hold",
    "-40dB to 6dB RMS 4 Sec.",
    "-40dB to 6dB PPM 4 Sec.",
    "-30dB to 6dB RMS 4 Sec.",
    "-30dB to 6dB PPM 4 Sec."};

class BarsVisualiser : public Visualiser
{
public:
    BarsVisualiser(TFT_eSPI *tft) : Visualiser(tft){};

    ~BarsVisualiser()
    {
        // tft->endWrite();
    }

    void init(void) override;
    void draw(int16_t valueLeft, int16_t valueRight) override;

private:
    void drawFFTLeftH(bool reset, bool fine);
    void drawFFTRightH(bool reset, bool fine);
    void drawFFTLeftV(bool reset, bool fine);
    void drawFFTRightV(bool reset, bool fine);
    void drawFFTBothV(bool reset, bool fine);
    uint16_t drawLeftBarFFT(bool reset, int16_t value);
    uint16_t drawRightBarFFT(bool reset, int16_t value);

    void setBins();
};

void BarsVisualiser::init(void)
{
    setBins();

    TFT_eSPI bg = *tft;

    // fill screen
    bg.fillScreen(FFT_ShowA700Frame ? TFT_BLACK : FFT_DisplayBackColor);

    // show the A700 frame
    if (FFT_ShowA700Frame)
        bg.fillRoundRect(0, 26, 240, 294, 7, FFT_DisplayBackColor);

    // show the calibration frame
    if (showCalibrationFrame)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_MAGENTA);

    // send calibration data
    if (calibrate)
        bg.drawRoundRect(0, 26, 240, 294, 7, TFT_RED);

    // write descriptions over bars
    if (FFT_LevelBarMode == 0)
        bg.drawString("PPM", 216, 36/*, FFT_HeaderForeColor, Arial_8, false*/);
    else
        bg.drawString("RMS", 215, 36/*, FFT_HeaderForeColor, Arial_8, false*/);

    // write bar 'dB' description
    float dBPre = 0.0f;
    for (uint16_t y = 0; y < 275; y++)
    {
        float dB = roundf((float)map(y, 0, 275, FFT_dBLow, 6.0f));
        if (dB != dBPre)
        {
            if (dB == 6)
            {
                bg.drawString("6", 223, 319 - y/*, TFT_RED, Arial_8, false*/);
            }
            if (dB == 3)
            {
                bg.drawString("3", 223, 319 - y/*, TFT_RED, Arial_8, false*/);
            }
            if (dB == 0)
            {
                bg.drawString("0", 223, 319 - y/*, TFT_RED, Arial_8, false*/);
            }
            if (dB == -5)
            {
                bg.drawString("5", 223, 319 - y/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            if (dB == -10)
            {
                bg.drawString("10", 220, 319 - y/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            if (dB == -20)
            {
                bg.drawString("20", 220, 319 - y/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            if (dB == -30)
            {
                bg.drawString("30", 220, 319 - y/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            if (dB == -40)
            {
                bg.drawString("40", 220, 319 - y/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            dBPre = dB;
        }
    }

    // vertical 2x25 bars (tree)
    if (FFT_BarMode == 0)
    {
        bg.drawFastVLine(94, 42, 276, FFT_LineColor);
        bg.drawFastVLine(120, 42, 276, FFT_LineColor);
        bg.fillRect(95, 42, 25, 276, FFT_ScaleBackgroundColor);
        bg.drawCentreString("SPECTRUM ANALYZER", 36, 215,/*, FFT_HeaderForeColor, Arial_8*/1);
        bg.drawString("L", 24, 42/*, FFT_LRColor, Arial_14_Bold, false*/);
        bg.drawString("R", 180, 42/*, FFT_LRColor, Arial_14_Bold, false*/);

        for (uint16_t x = 0; x < 25; x++)
        {
            float freq = 43.0f + 43.0f * (binStart25[x] + ((binEnd25[x] - binStart25[x]) / 2.));

            uint16_t yPos = 315 - map(x, 0, 25, 0, 275);
            if (freq >= 10000.0f)
            {
                bg.drawNumber(round(freq / 1000.0f), 99, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                bg.drawString("k", 112, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            else if (freq >= 1000.0f)
            {
                bg.drawFloat(freq / 1000.0f, 1, 98, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                bg.drawString("k", 114, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            else if (freq >= 100.0f)
            {
                bg.drawNumber(freq, 99, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
            else if (freq >= 10.0f)
            {
                bg.drawNumber(freq, 102, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
            else
            {
                bg.drawNumber(freq, 105, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
        }
        // vertical 2x69 bars (tree)
    }
    else if (FFT_BarMode == 3)
    {
        bg.drawFastVLine(94, 42, 276, FFT_LineColor);
        bg.drawFastVLine(120, 42, 276, FFT_LineColor);
        bg.fillRect(95, 42, 25, 276, FFT_ScaleBackgroundColor);
        // bg.drawCentreString("HI-RES ANALYZER", 36, 215, 1/*, FFT_HeaderForeColor, Arial_8*/);
        bg.drawString("L", 24, 42/*, FFT_LRColor, Arial_14_Bold, false*/);
        bg.drawString("R", 180, 42/*, FFT_LRColor, Arial_14_Bold, false*/);

        for (uint16_t x = 0; x < 69; x++)
        {
            if (x % 3 == 0)
            {
                float freq = 43.0f * (binStart69[x + 1] + ((binEnd69[x + 1] - binStart69[x + 1]) / 2.0f));
                uint16_t yPos = 315 - map(x, 0, 69, 0, 275);
                if (freq >= 10000.0f)
                {
                    bg.drawNumber(round(freq / 1000.0f), 99, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                    bg.drawString("k", 112, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
                }
                else if (freq >= 1000.0f)
                {
                    bg.drawFloat(freq / 1000.0f, 1, 98, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                    bg.drawString("k", 114, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
                }
                else if (freq >= 100.0f)
                {
                    bg.drawNumber(freq, 99, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
                else if (freq >= 10.0f)
                {
                    bg.drawNumber(freq, 102, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
                else
                {
                    bg.drawNumber(freq, 105, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
            }
        }

        // horizontal 16 bars
    }
    else if (FFT_BarMode == 1)
    {
        bg.drawCentreString("SPECTRUM ANALYZER", 36, 215/*, FFT_HeaderForeColor, Arial_8*/,1);
        bg.drawFastHLine(2, 170, 207, FFT_LineColor);
        bg.drawFastHLine(2, 315, 207, FFT_LineColor);
        bg.fillRect(2, 171, 206, 13, FFT_ScaleBackgroundColor);

        for (uint16_t x = 0; x < 16; x++)
        {
            if (x % 2 == 0)
            {
                float freq = 43.0f * (binStart16[x + 1] + ((binEnd16[x + 1] - binStart16[x + 1]) / 2.0f));
                uint16_t vSep = map(x, 0, 16, 2, 206);
                if (x > 12)
                    vSep -= 4;
                bg.drawFastVLine(vSep, 171, 13, FFT_LineColor);
                char buf[4];
                (freq >= 1000.0f) ? sprintf(buf, "%u%c\n", (uint16_t)round(freq / 1000.0f), 'k') : sprintf(buf, "%u", (uint16_t)freq);
                bg.drawString(buf, vSep + 3, 182/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
        }

        bg.drawString("L", 8, 66/*, FFT_LRColor, Arial_14_Bold, false*/);
        bg.drawString("R", 8, 210/*, FFT_LRColor, Arial_14_Bold, false*/);

        // horizontal 52 bars
    }
    else if (FFT_BarMode == 4)
    {
        // bg.drawCentreString("HI-RES ANALYZER", 36, 215, 1/*FFT_HeaderForeColor, Arial_8*/);
        bg.drawFastHLine(2, 170, 207, FFT_LineColor);
        bg.drawFastHLine(2, 315, 207, FFT_LineColor);
        bg.fillRect(2, 171, 206, 13, FFT_ScaleBackgroundColor);

        for (uint16_t x = 0; x < 52; x++)
        {
            if (x % 6 == 0)
            {
                float freq = 43.0f * (binStart52[x + 1] + ((binEnd52[x + 1] - binStart52[x + 1]) / 2.0f));
                uint16_t vSep = map(x, 0, 52, 2, 206);
                if (x > 12)
                    vSep -= 4;
                bg.drawFastVLine(vSep, 171, 13, FFT_LineColor);
                char buf[4];
                (freq >= 1000.0f) ? sprintf(buf, "%u%c\n", (uint16_t)round(freq / 1000.0f), 'k') : sprintf(buf, "%u", (uint16_t)freq);
                bg.drawString(buf, vSep + 3, 182/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
        }

        bg.drawString("L", 2, 66/*, FFT_LRColor, Arial_14_Bold, false*/);
        bg.drawString("R", 2, 210/*, FFT_LRColor, Arial_14_Bold, false*/);

        // sum vertical 25 bars
    }
    else if (FFT_BarMode == 2)
    {
        bg.drawFastVLine(2, 42, 276, FFT_LineColor);
        bg.drawFastVLine(28, 42, 276, FFT_LineColor);
        bg.fillRect(3, 42, 25, 276, FFT_ScaleBackgroundColor);
        bg.drawCentreString("SPECTRUM ANALYZER", 36, 215, 1/*FFT_HeaderForeColor, Arial_8*/);

        for (uint16_t x = 0; x < 25; x++)
        {
            float freq = 43.0f + 43.0f * (binStart25[x] + ((binEnd25[x] - binStart25[x]) / 2.0f));
            uint16_t yPos = 315 - map(x, 0, 25, 0, 275);

            if (freq >= 10000.0f)
            {
                bg.drawNumber(round(freq / 1000.0f), 7, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                bg.drawString("k", 20, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            else if (freq >= 1000.0f)
            {
                bg.drawFloat(freq / 1000.0f, 1, 6, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                bg.drawString("k", 21, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
            }
            else if (freq >= 100.0f)
            {
                bg.drawNumber(freq, 7, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
            else if (freq >= 10.0f)
            {
                bg.drawNumber(freq, 10, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
            else
            {
                bg.drawNumber(freq, 13, yPos/*, FFT_ScaleForeColor, Arial_8*/);
            }
        }

        // sum vertical 69 bars
    }
    else if (FFT_BarMode == 5)
    {
        bg.drawFastVLine(2, 42, 276, FFT_LineColor);
        bg.drawFastVLine(28, 42, 276, FFT_LineColor);
        bg.fillRect(3, 42, 25, 276, FFT_ScaleBackgroundColor);
        // bg.drawCentreString("HI-RES ANALYZER", 36, 215, 1/*FFT_HeaderForeColor, Arial_8*/);

        for (uint16_t x = 0; x < 69; x++)
        {
            if (x % 3 == 0)
            {
                float freq = 43.0f * (binStart69[x + 1] + ((binEnd69[x + 1] - binStart69[x + 1]) / 2.0f));
                uint16_t yPos = 315 - map(x, 0, 69, 0, 275);

                if (freq >= 10000.0f)
                {
                    bg.drawNumber(round(freq / 1000.0f), 7, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                    bg.drawString("k", 20, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
                }
                else if (freq >= 1000.0f)
                {
                    bg.drawFloat(freq / 1000.0f, 1, 6, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                    bg.drawString("k", 21, yPos/*, FFT_ScaleForeColor, Arial_8, false*/);
                }
                else if (freq >= 100.0f)
                {
                    bg.drawNumber(freq, 7, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
                else if (freq >= 10.0f)
                {
                    bg.drawNumber(freq, 10, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
                else
                {
                    bg.drawNumber(freq, 13, yPos/*, FFT_ScaleForeColor, Arial_8*/);
                }
            }
        }
    }

    // copy background to front
    // memcpy(frontBuffer, backBuffer, 153600);
    // drawFFT(reset);
    // tft.update(frontBuffer, true);
}

void BarsVisualiser::draw(int16_t valueLeft, int16_t valueRight)
{
    bool reset = false; 

    drawLeftBarFFT(reset, valueLeft);
    drawRightBarFFT(reset, valueRight);

    switch (FFT_BarMode)
    {
    case 0:
        drawFFTLeftV(reset, false);
        drawFFTRightV(reset, false);
        break;
    case 1:
        drawFFTLeftH(reset, false);
        drawFFTRightH(reset, false);
        break;
    case 2:
        drawFFTBothV(reset, false);
        break;
    case 3:
        drawFFTLeftV(reset, true);
        drawFFTRightV(reset, true);
        break;
    case 4:
        drawFFTLeftH(reset, true);
        drawFFTRightH(reset, true);
        break;
    case 5:
        drawFFTBothV(reset, true);
        break;
    }
}

/// <summary>
/// draw horizontal FFT for left channel
/// </summary>
/// <param name="reset"></param>
/// <param name="fine">true=52, false=16</param>
void BarsVisualiser::drawFFTLeftH(bool reset, bool fine)
{
    TFT_eSPI im = *tft;

    // Static vars
    static uint32_t clearMaxHold = 0;
    static uint16_t bar = 0;
    static uint16_t pastData[52] = {0};
    static uint16_t peakHoldCounter[52] = {0};
    static uint16_t fallingCounter[52] = {0};
    static uint16_t peakHoldMax[52] = {0};

    // vars
    float n = 0.0f;                          // FFT value in dB
    uint16_t posY = 169;                     // y position
    uint16_t barCount = fine ? 52 : 16;      // bars count
    uint16_t barLimit = 128;                 // bar length limit
    uint16_t barWideTotal = fine ? 4 : 13;   // total bar wide
    uint16_t barWideVisible = fine ? 3 : 11; // visible bar wide
    uint16_t rainbowSteps = fine ? 2 : 9;    // for rainbow colors

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
    {
        for (uint16_t i = 0; i < 52; i++)
            peakHoldMax[i] = 0;
    }

    if (fft1024_1.available())
    {
        for (bar = 0; bar < barCount; bar++)
        {

            /* READ & SCALE SECTION
             *************************************************/
            if (fine)
            {
                n = fftAmplitudeTodB(fft1024_1.read(binStart52[bar], binEnd52[bar]));
            }
            else
            {
                n = fftAmplitudeTodB(fft1024_1.read(binStart16[bar], binEnd16[bar]));
            }

            uint16_t val = map(n, FFT_dBLow, dBHigh, 0, barLimit);

            // reset maxhold counter.
            if (val > 0)
                clearMaxHold = 0;

            /* BALLISTICS SECTION
             *************************************************/
            // Get previous bar data.
            uint16_t pastVal = pastData[bar];

            // Meter ballistics.
            if (val < pastVal - FFT_FFTBarRelease)
                val = pastVal - FFT_FFTBarRelease;
            if (pastVal < val - FFT_FFTBarAttack)
                val = pastVal + FFT_FFTBarAttack;
            if (++peakHoldCounter[bar] > FFT_FFTPeakHoldTime && fallingCounter[bar] > FFT_FFTPeakRelease)
                fallingCounter[bar] -= FFT_FFTPeakRelease;

            // Store bar data.
            pastData[bar] = val;

            // store new peak level and reset the peak hold counter
            if (val > fallingCounter[bar])
            {
                fallingCounter[bar] = val + 1; // +1 is very important!
                peakHoldCounter[bar] = 0;
            }

            // store new max level for peakhold
            if (val > peakHoldMax[bar])
            {
                peakHoldMax[bar] = val + 1; // +1 is very important!
            }

            /* DRAW BARS SECTION
             *************************************************/
            for (int y = 0; y < barLimit; y++)
            {
                uint16_t color = (FFT_UseWheel) ? rainbow(bar * rainbowSteps) : FFT_FFTBarColor;

                // draw bars.
                if (y < pastVal)
                {
                    im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideVisible, (FFT_FFTSegmentHeight == 0) ? color : (y % (FFT_FFTSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                                                                   : color); // bars
                    im.drawFastHLine(2 + barWideVisible + bar * barWideTotal, posY - y, barWideTotal - barWideVisible, FFT_DisplayBackColor);                                // clear gap
                }
                else
                {
                    if (!FFT_PeakHold && y > 10 /*ShowFromPoint*/ && y >= fallingCounter[bar] && y < fallingCounter[bar] + FFT_FFTPeakHeight)
                        im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideVisible, FFT_FFTPeakColor); // peaks
                    else
                        im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideTotal, FFT_DisplayBackColor); // clear
                }
            }
        }
    }

    // draw peak graph
    if (FFT_PeakHold)
    {
        for (uint16_t b = 0; b < barCount - 1; b++)
        {
            im.drawLine(2 + (b * barWideTotal) + (barWideTotal / 2), posY - peakHoldMax[b], 2 + ((b + 1) * barWideTotal) + (barWideTotal / 2), posY - peakHoldMax[b + 1], FFT_FFTPeakColor);
        }
    }

    im.drawString("L", 8, 66/*, FFT_LRColor, Arial_14_Bold, false*/);
}

/// <summary>
/// draw horizontal FFT for right channel
/// </summary>
/// <param name="reset"></param>
/// <param name="fine">true=52, false=16</param>
void BarsVisualiser::drawFFTRightH(bool reset, bool fine)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static uint16_t bar = 0;
    static uint16_t pastData[52] = {0};
    static uint16_t peakHoldCounter[52] = {0};
    static uint16_t fallingCounter[52] = {0};
    static uint16_t peakHoldMax[52] = {0};

    // vars
    float n = 0.0f;                          // FFT value in dB
    uint16_t posY = 314;                     // y position
    uint16_t barCount = fine ? 52 : 16;      // bars count
    uint16_t barLimit = 128;                 // bar length limit
    uint16_t barWideTotal = fine ? 4 : 13;   // total bar wide
    uint16_t barWideVisible = fine ? 3 : 11; // visible bar wide
    uint16_t rainbowSteps = fine ? 2 : 9;    // for rainbow colors

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
    {
        for (uint16_t i = 0; i < 52; i++)
            peakHoldMax[i] = 0;
    }

    if (fft1024_2.available())
    {
        for (bar = 0; bar < barCount; bar++)
        {

            /* READ & SCALE SECTION
             *************************************************/
            if (fine)
            {
                n = fftAmplitudeTodB(fft1024_2.read(binStart52[bar], binEnd52[bar]));
            }
            else
            {
                n = fftAmplitudeTodB(fft1024_2.read(binStart16[bar], binEnd16[bar]));
            }

            uint16_t val = map(n, FFT_dBLow, dBHigh, 0, barLimit);

            // reset maxhold counter
            if (val > 0)
                clearMaxHold = 0;

            /* BALLISTICS SECTION
             *************************************************/
            // get previous bar data
            uint16_t pastVal = pastData[bar];

            // meter ballistics
            if (val < pastVal - FFT_FFTBarRelease)
                val = pastVal - FFT_FFTBarRelease;
            if (pastVal < val - FFT_FFTBarAttack)
                val = pastVal + FFT_FFTBarAttack;
            if (++peakHoldCounter[bar] > FFT_FFTPeakHoldTime && fallingCounter[bar] > FFT_FFTPeakRelease)
                fallingCounter[bar] -= FFT_FFTPeakRelease;

            // store bar data
            pastData[bar] = val;

            // store new peak level and reset the peak hold counter
            if (val > fallingCounter[bar])
            {
                fallingCounter[bar] = val + 1; // +1 is very important!
                peakHoldCounter[bar] = 0;
            }

            // store new max level for peakhold
            if (val > peakHoldMax[bar])
            {
                peakHoldMax[bar] = val + 1; // +1 is very important!
            }

            /* DRAW BARS SECTION
             *************************************************/
            for (int y = 0; y < barLimit; y++)
            {
                uint16_t color = (FFT_UseWheel) ? rainbow(bar * rainbowSteps) : FFT_FFTBarColor;

                // draw bars
                if (y < pastVal)
                {
                    im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideVisible, (FFT_FFTSegmentHeight == 0) ? color : (y % (FFT_FFTSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                                                                   : color); // bars
                    im.drawFastHLine(2 + barWideVisible + bar * barWideTotal, posY - y, barWideTotal - barWideVisible, FFT_DisplayBackColor);                                // clear gap
                }
                else
                {
                    if (!FFT_PeakHold && y > 10 /*ShowFromPoint*/ && y >= fallingCounter[bar] && y < fallingCounter[bar] + FFT_FFTPeakHeight)
                        im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideVisible, FFT_FFTPeakColor); // peaks
                    else
                        im.drawFastHLine(2 + bar * barWideTotal, posY - y, barWideTotal, FFT_DisplayBackColor); // clear
                }
            }
        }
    }

    // draw peak graph
    if (FFT_PeakHold)
    {
        for (uint16_t b = 0; b < barCount - 1; b++)
        {
            im.drawLine(2 + (b * barWideTotal) + (barWideTotal / 2), posY - peakHoldMax[b], 2 + ((b + 1) * barWideTotal) + (barWideTotal / 2), posY - peakHoldMax[b + 1], FFT_FFTPeakColor);
        }
    }

    im.drawString("R", 8, 210/*, FFT_LRColor, Arial_14_Bold, false*/);
}

/// <summary>
/// draw vertical FFT for left channel
/// </summary>
/// <param name="reset"></param>
/// <param name="fine">true=69, false=25</param>
void BarsVisualiser::drawFFTLeftV(bool reset, bool fine)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static uint16_t bar = 0;
    static uint16_t pastData[69] = {0};
    static uint16_t peakHoldCounter[69] = {0};
    static uint16_t fallingCounter[69] = {0};
    static uint16_t peakHoldMax[69] = {0};

    // vars
    float n = 0.0f;                           // FFT value in dB
    uint16_t posX = 93;                       // x position
    uint16_t posY = fine ? 315 : 307;         // y position
    uint16_t barCount = fine ? 69 : 25;       // bars count
    uint16_t barLimit = 90;                   // bar length limit
    uint16_t barHeightTotal = fine ? 4 : 11;  // total bar height
    uint16_t barHeightVisible = fine ? 3 : 9; // visible bar height
    uint16_t rainbowSteps = fine ? 2 : 6;     // for rainbow colors

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
    {
        for (uint16_t i = 0; i < 69; i++)
            peakHoldMax[i] = 0;
    }

    if (fft1024_1.available())
    {
        for (bar = 0; bar < barCount; bar++)
        {

            /* READ & SCALE SECTION
             *************************************************/
            if (fine)
            {
                n = fftAmplitudeTodB(fft1024_1.read(binStart69[bar], binEnd69[bar]), FFT_dBLow);
            }
            else
            {
                n = fftAmplitudeTodB(fft1024_1.read(binStart25[bar], binEnd25[bar]), FFT_dBLow);
            }

            uint16_t val = map(n, FFT_dBLow, dBHigh, 0, barLimit);
            // Serial.printf("n = %.2f, val = %d \n", n, val);

            // reset maxhold counter
            if (val > 0)
                clearMaxHold = 0;

            /* BALLISTICS SECTION
             *************************************************/
            // get previous bar data
            uint16_t pastVal = pastData[bar];

            // meter ballistics
            if (val < pastVal - FFT_FFTBarRelease)
                val = pastVal - FFT_FFTBarRelease;
            if (pastVal < val - FFT_FFTBarAttack)
                val = pastVal + FFT_FFTBarAttack;
            if (++peakHoldCounter[bar] > FFT_FFTPeakHoldTime && fallingCounter[bar] > FFT_FFTPeakRelease)
                fallingCounter[bar] -= FFT_FFTPeakRelease;

            // store bar data
            pastData[bar] = val;

            // store new peak level and reset the peak hold counter
            if (val > fallingCounter[bar])
            {
                fallingCounter[bar] = val + 1; // +1 is very important!
                peakHoldCounter[bar] = 0;
            }

            // store new max level for peakhold
            if (val > peakHoldMax[bar])
            {
                peakHoldMax[bar] = val + 1; // +1 is very important!
            }

            /* DRAW BARS SECTION
             *************************************************/
            for (int x = 0; x < barLimit; x++)
            {
                uint16_t color = (FFT_UseWheel) ? rainbow(bar * rainbowSteps) : FFT_FFTBarColor;

                // draw bars
                if (x < pastVal)
                {
                    im.drawFastVLine(posX - x, posY - bar * barHeightTotal, barHeightVisible, (FFT_FFTSegmentHeight == 0) ? color : (x % (FFT_FFTSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                                                                          : color); // bars
                    im.drawFastVLine(posX - x, posY - bar * barHeightTotal + barHeightVisible, barHeightTotal - barHeightVisible, FFT_DisplayBackColor);                            // clear gap
                }
                else
                {
                    if (!FFT_PeakHold && x > 10 /*ShowFromPoint*/ && x >= fallingCounter[bar] && x < fallingCounter[bar] + FFT_FFTPeakHeight)
                        im.drawFastVLine(posX - x, posY - bar * barHeightTotal, barHeightVisible, FFT_FFTPeakColor); // peaks
                    else
                        im.drawFastVLine(posX - x, posY - bar * barHeightTotal, barHeightTotal, FFT_DisplayBackColor); // clear
                }
            }
        }
    }

    // draw peak graph
    if (FFT_PeakHold)
    {
        for (uint16_t b = 0; b < barCount - 1; b++)
        {
            im.drawLine(posX - peakHoldMax[b], posY - (b * barHeightTotal) + (barHeightTotal / 2), posX - peakHoldMax[b + 1], posY - ((b + 1) * barHeightTotal) + (barHeightTotal / 2), FFT_FFTPeakColor);
        }
    }
}

/// <summary>
/// draw vertical FFT for right channel
/// </summary>
/// <param name="reset"></param>
/// <param name="fine">true=69, false=25</param>
void BarsVisualiser::drawFFTRightV(bool reset, bool fine)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static uint16_t bar = 0;
    static uint16_t pastData[69] = {0};
    static uint16_t peakHoldCounter[69] = {0};
    static uint16_t fallingCounter[69] = {0};
    static uint16_t peakHoldMax[69] = {0};

    // vars
    float n = 0.0f;                           // FFT value in dB
    uint16_t posX = 122;                      // x position
    uint16_t posY = fine ? 315 : 307;         // y position
    uint16_t barCount = fine ? 69 : 25;       // bars count
    uint16_t barLimit = 90;                   // bar length limit
    uint16_t barHeightTotal = fine ? 4 : 11;  // total bar height
    uint16_t barHeightVisible = fine ? 3 : 9; // visible bar height
    uint16_t rainbowSteps = fine ? 2 : 6;     // for rainbow colors

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
    {
        for (uint16_t i = 0; i < 69; i++)
            peakHoldMax[i] = 0;
    }

    if (fft1024_2.available())
    {
        for (bar = 0; bar < barCount; bar++)
        {

            /* READ & SCALE SECTION
             *************************************************/
            if (fine)
            {
                n = fftAmplitudeTodB(fft1024_2.read(binStart69[bar], binEnd69[bar]));
            }
            else
            {
                n = fftAmplitudeTodB(fft1024_2.read(binStart25[bar], binEnd25[bar]));
            }

            uint16_t val = map(n, FFT_dBLow, dBHigh, 0, barLimit);

            // reset maxhold counter
            if (val > 0)
                clearMaxHold = 0;

            /* BALLISTICS SECTION
             *************************************************/
            // get previous bar data
            uint16_t pastVal = pastData[bar];

            // meter ballistics
            if (val < pastVal - FFT_FFTBarRelease)
                val = pastVal - FFT_FFTBarRelease;
            if (pastVal < val - FFT_FFTBarAttack)
                val = pastVal + FFT_FFTBarAttack;
            if (++peakHoldCounter[bar] > FFT_FFTPeakHoldTime && fallingCounter[bar] > FFT_FFTPeakRelease)
                fallingCounter[bar] -= FFT_FFTPeakRelease;

            // store bar data
            pastData[bar] = val;

            // store new peak level and reset the peak hold counter
            if (val > fallingCounter[bar])
            {
                fallingCounter[bar] = val + 1; // +1 is very important!
                peakHoldCounter[bar] = 0;
            }

            // store new max level for peakhold
            if (val > peakHoldMax[bar])
            {
                peakHoldMax[bar] = val + 1; // +1 is very important!
            }

            /* DRAW BARS SECTION
             *************************************************/
            for (int x = 0; x < barLimit; x++)
            {
                uint16_t color = (FFT_UseWheel) ? rainbow(bar * rainbowSteps) : FFT_FFTBarColor;

                // draw bars
                if (x < pastVal)
                {
                    im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightVisible, (FFT_FFTSegmentHeight == 0) ? color : (x % (FFT_FFTSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                                                                          : color); // bars
                    im.drawFastVLine(posX + x, posY - bar * barHeightTotal + barHeightVisible, barHeightTotal - barHeightVisible, FFT_DisplayBackColor);                            // clear gap
                }
                else
                {
                    if (!FFT_PeakHold && x > 10 /*ShowFromPoint*/ && x >= fallingCounter[bar] && x < fallingCounter[bar] + FFT_FFTPeakHeight)
                        im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightVisible, FFT_FFTPeakColor); // peaks
                    else
                        im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightTotal, FFT_DisplayBackColor); // clear
                }
            }
        }
    }

    // draw peak graph
    if (FFT_PeakHold)
    {
        for (uint16_t b = 0; b < barCount - 1; b++)
        {
            im.drawLine(posX + peakHoldMax[b], posY - (b * barHeightTotal) + (barHeightTotal / 2), posX + peakHoldMax[b + 1], posY - ((b + 1) * barHeightTotal) + (barHeightTotal / 2), FFT_FFTPeakColor);
        }
    }
}

/// <summary>
/// draw vertical 69 or 25 bars for both channels
/// </summary>
/// <param name="reset"></param>
/// <param name="fine">true=69, false=25</param>
void BarsVisualiser::drawFFTBothV(bool reset, bool fine)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static uint16_t bar = 0;
    static uint16_t pastData[69] = {0};
    static uint16_t peakHoldCounter[69] = {0};
    static uint16_t fallingCounter[69] = {0};
    static uint16_t peakHoldMax[69] = {0};

    // vars
    float n = 0.0f;                           // FFT value in dB
    uint16_t posX = 30;                       // x position
    uint16_t posY = fine ? 315 : 307;         // y position
    uint16_t barCount = fine ? 69 : 25;       // bars count
    uint16_t barLimit = 180;                  // bar length limit
    uint16_t barHeightTotal = fine ? 4 : 11;  // total bar height
    uint16_t barHeightVisible = fine ? 3 : 9; // visible bar height
    uint16_t rainbowSteps = fine ? 2 : 6;     // for rainbow colors

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
    {
        for (uint16_t i = 0; i < 69; i++)
            peakHoldMax[i] = 0;
    }

    if (fft1024_3.available())
    {
        for (bar = 0; bar < barCount; bar++)
        {

            /* READ & SCALE SECTION
             *************************************************/
            if (fine)
            {
                n = fftAmplitudeTodB(fft1024_3.read(binStart69[bar], binEnd69[bar]));
            }
            else
            {
                n = fftAmplitudeTodB(fft1024_3.read(binStart25[bar], binEnd25[bar]));
            }

            uint16_t val = map(n, FFT_dBLow, dBHigh, 0, barLimit);

            // reset maxhold counter
            if (val > 0)
                clearMaxHold = 0;

            /* BALLISTICS SECTION
             *************************************************/
            // get previous bar data
            uint16_t pastVal = pastData[bar];

            // meter ballistics
            if (val < pastVal - FFT_FFTBarRelease)
                val = pastVal - FFT_FFTBarRelease;
            if (pastVal < val - FFT_FFTBarAttack)
                val = pastVal + FFT_FFTBarAttack;
            if (++peakHoldCounter[bar] > FFT_FFTPeakHoldTime && fallingCounter[bar] > FFT_FFTPeakRelease)
                fallingCounter[bar] -= FFT_FFTPeakRelease;

            // store bar data
            pastData[bar] = val;

            // store new peak level and reset the peak hold counter
            if (val > fallingCounter[bar])
            {
                fallingCounter[bar] = val + 1; // +1 is very important!
                peakHoldCounter[bar] = 0;
            }

            // store new max level for peakhold.
            if (val > peakHoldMax[bar])
            {
                peakHoldMax[bar] = val + 1; // +1 is very important!
            }

            /* DRAW BARS SECTION
             *************************************************/
            for (int x = 0; x < barLimit; x++)
            {
                uint16_t color = (FFT_UseWheel) ? rainbow(bar * rainbowSteps) : FFT_FFTBarColor;

                // draw bars
                if (x < pastVal)
                {
                    im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightVisible, (FFT_FFTSegmentHeight == 0) ? color : (x % (FFT_FFTSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                                                                          : color); // bars
                    im.drawFastVLine(posX + x, posY - bar * barHeightTotal + barHeightVisible, barHeightTotal - barHeightVisible, FFT_DisplayBackColor);                            // clear gap
                }
                else
                {
                    if (!FFT_PeakHold && x > 10 /*ShowFromPoint*/ && x >= fallingCounter[bar] && x < fallingCounter[bar] + FFT_FFTPeakHeight)
                        im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightVisible, FFT_FFTPeakColor); // peaks
                    else
                        im.drawFastVLine(posX + x, posY - bar * barHeightTotal, barHeightTotal, FFT_DisplayBackColor); // clear
                }
            }
        }
    }

    // draw peak graph
    if (FFT_PeakHold)
    {
        for (uint16_t b = 0; b < barCount - 1; b++)
        {
            im.drawLine(posX + peakHoldMax[b], posY - (b * barHeightTotal) + (barHeightTotal / 2), posX + peakHoldMax[b + 1], posY - ((b + 1) * barHeightTotal) + (barHeightTotal / 2), FFT_FFTPeakColor);
        }
    }
}

/// <summary>
/// draw left bar
/// </summary>
/// <param name="reset"></param>
/// <returns>bar length</returns>
uint16_t BarsVisualiser::drawLeftBarFFT(bool reset, int16_t value)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static float dBPre = 0.0f;
    static uint16_t pastVal = 0, holdCounter = 0, fallingCounter = 0, peakHoldMax = 0;

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
        peakHoldMax = 0;

    if (value /*(FFT_LevelBarMode == 0) ? peak1.available() : rms1.available()*/)
    {
        uint16_t rmsVal = map(rmsAmplitudeTodB(value/*rms2.read()*/, 0), FFT_dBLow, dBHigh, 0, 275);
        uint16_t peakVal = map(peakAmplitudeTodB(value/*peak2.read()*/, 0, FFT_PeakAttenuation), FFT_dBLow, dBHigh, 0, 275);

        // read RMS or PEAK, calculate dB & scale dB range to bar length
        uint16_t barVal = (FFT_LevelBarMode == 0) ? rmsVal : peakVal;

        // reset maxhold counter
        if (barVal > 0)
            clearMaxHold = 0;

        // get previous bar data
        uint16_t valTmp = pastVal;

        // ballistics
        if (FFT_LevelBarMode == 0 /*PEAK*/)
        {
            if (barVal < valTmp - FFT_PEAKBarRelease)
                barVal = valTmp - FFT_PEAKBarRelease;
            if (valTmp < barVal - FFT_PEAKBarAttack)
                barVal = valTmp + FFT_PEAKBarAttack;
            if (++holdCounter > FFT_PEAKPeakHoldTime && fallingCounter > FFT_PEAKPeakRelease)
                fallingCounter -= FFT_PEAKPeakRelease; // peak release
        }
        else
        {
            if (barVal < valTmp - FFT_VUBarRelease)
                barVal = valTmp - FFT_VUBarRelease;
            if (valTmp < barVal - FFT_VUBarAttack)
                barVal = valTmp + FFT_VUBarAttack;
            if (++holdCounter > FFT_VUPeakHoldTime && fallingCounter > FFT_VUPeakRelease)
                fallingCounter -= FFT_VUPeakRelease; // peak release
        }

        // store bar data
        pastVal = barVal;

        // store new max level and reset the hold counter
        if (barVal > fallingCounter)
        {
            fallingCounter = barVal + 1; // +1 is very important!
            holdCounter = 0;
        }

        // store new max level for peakhold
        if (barVal > peakHoldMax)
        {
            peakHoldMax = barVal + 1; // +1 is very important!
        }

        // draw bars and peaks
        for (uint16_t y = 0; y < 275; y++)
        {
            float dB = roundf(map((float)y, 0, 275, FFT_dBLow, 6.0f));

            // colors
            uint16_t color = FFT_VUBarColorNormal;
            if (dB >= 0.0f)
                color = FFT_VUBarColorOver;
            else if (dB >= -5.0f)
                color = FFT_VUBarColorMid;

            // bars
            if (y < pastVal)
            {
                im.drawFastHLine(214, 315 - y, 4, (FFT_VUSegmentHeight == 0) ? color : (y % (FFT_VUSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                            : color); // bars
            }
            else
            {
                if (FFT_PeakHold)
                {
                    im.drawFastHLine(214, 315 - y, 4, (y > 10 && y >= peakHoldMax && y < peakHoldMax + FFT_VUPeakHeight) ? FFT_VUPeakColor : FFT_VUBarsBackColor);
                }
                else
                {
                    im.drawFastHLine(214, 315 - y, 4, (y > 10 && y >= fallingCounter && y < fallingCounter + FFT_VUPeakHeight) ? FFT_VUPeakColor : FFT_VUBarsBackColor);
                }
            }

            // markers
            if (dB != dBPre)
            {
                if (dB == 6.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 5.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 4.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 3.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 2.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 1.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == 0.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, TFT_RED);
                }
                if (dB == -5.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -10.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -15.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -20.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -25.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -30.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -35.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -40.0f)
                {
                    im.drawFastHLine(214, 315 - y, 4, FFT_VUMarkerColor);
                }
                dBPre = dB;
            }
        }
        return barVal;
    }
    return 0;
}

/// <summary>
/// draw right bar
/// </summary>
/// <param name="reset"></param>
/// <returns>bar length</returns>
uint16_t BarsVisualiser::drawRightBarFFT(bool reset, int16_t value)
{
    TFT_eSPI im = *tft;

    // static vars
    static uint32_t clearMaxHold = 0;
    static float dBPre = 0.0f;
    static uint16_t pastVal = 0, holdCounter = 0, fallingCounter = 0, peakHoldMax = 0;

    // clear peak hold max
    if (reset || (FFT_WorkMode > 7 && clearMaxHold > 4000))
        peakHoldMax = 0;

    if (value/*(FFT_LevelBarMode == 0) ? peak2.available() : rms2.available()*/)
    {
        uint16_t rmsVal = map(rmsAmplitudeTodB(value/*rms2.read()*/, 1), FFT_dBLow, dBHigh, 0, 275);
        uint16_t peakVal = map(peakAmplitudeTodB(value/*peak2.read()*/, 1, FFT_PeakAttenuation), FFT_dBLow, dBHigh, 0, 275);

        // read RMS or PEAK, calculate dB & scale dB range to bar length
        uint16_t barVal = (FFT_LevelBarMode == 0) ? rmsVal : peakVal;

        // reset maxhold counter
        if (barVal > 0)
            clearMaxHold = 0;

        // get previous bar data
        uint16_t valTmp = pastVal;

        // ballistics
        if (FFT_LevelBarMode == 0 /*PEAK*/)
        {
            if (barVal < valTmp - FFT_PEAKBarRelease)
                barVal = valTmp - FFT_PEAKBarRelease;
            if (valTmp < barVal - FFT_PEAKBarAttack)
                barVal = valTmp + FFT_PEAKBarAttack;
            if (++holdCounter > FFT_PEAKPeakHoldTime && fallingCounter > FFT_PEAKPeakRelease)
                fallingCounter -= FFT_PEAKPeakRelease; // peak release
        }
        else
        {
            if (barVal < valTmp - FFT_VUBarRelease)
                barVal = valTmp - FFT_VUBarRelease;
            if (valTmp < barVal - FFT_VUBarAttack)
                barVal = valTmp + FFT_VUBarAttack;
            if (++holdCounter > FFT_VUPeakHoldTime && fallingCounter > FFT_VUPeakRelease)
                fallingCounter -= FFT_VUPeakRelease; // peak release
        }

        // store bar data
        pastVal = barVal;

        // store new max level and reset the hold counter
        if (barVal > fallingCounter)
        {
            fallingCounter = barVal + 1; // +1 is very important!
            holdCounter = 0;
        }

        // store new max level for peakhold
        if (barVal > peakHoldMax)
        {
            peakHoldMax = barVal + 1; // +1 is very important!
        }

        // draw bars and peaks
        for (uint16_t y = 0; y < 275; y++)
        {
            float dB = roundf(map((float)y, 0, 275, FFT_dBLow, 6.0f));

            // colors
            uint16_t color = FFT_VUBarColorNormal;
            if (dB >= 0.0f)
                color = FFT_VUBarColorOver;
            else if (dB >= -5.0f)
                color = FFT_VUBarColorMid;

            // bars
            if (y < pastVal)
            {
                im.drawFastHLine(233, 315 - y, 4, (FFT_VUSegmentHeight == 0) ? color : (y % (FFT_VUSegmentHeight + 1) == 0) ? FFT_DisplayBackColor
                                                                                                                            : color); // bars
            }
            else
            {
                if (FFT_PeakHold)
                {
                    im.drawFastHLine(233, 315 - y, 4, (y > 10 && y >= peakHoldMax && y < peakHoldMax + FFT_VUPeakHeight) ? FFT_VUPeakColor : FFT_VUBarsBackColor);
                }
                else
                {
                    im.drawFastHLine(233, 315 - y, 4, (y > 10 && y >= fallingCounter && y < fallingCounter + FFT_VUPeakHeight) ? FFT_VUPeakColor : FFT_VUBarsBackColor);
                }
            }

            // markers
            if (dB != dBPre)
            {
                if (dB == 6.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 5.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 4.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 3.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 2.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 1.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == 0.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, TFT_RED);
                }
                if (dB == -5.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -10.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -15.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -20.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -25.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -30.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -35.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                if (dB == -40.0f)
                {
                    im.drawFastHLine(233, 315 - y, 4, FFT_VUMarkerColor);
                }
                dBPre = dB;
            }
        }
        return barVal;
    }
    return 0;
}


/// <summary>
/// set bin ranges
/// </summary>
void BarsVisualiser::setBins()
{
  // frequency range for the 16 horizontal bars
  binStart16[0] = 0;
  binEnd16[0] = 0;
  binStart16[1] = 1;
  binEnd16[1] = 1;
  binStart16[2] = 2;
  binEnd16[2] = 3;
  binStart16[3] = 4;
  binEnd16[3] = 6;
  binStart16[4] = 7;
  binEnd16[4] = 10;
  binStart16[5] = 11;
  binEnd16[5] = 15;
  binStart16[6] = 16;
  binEnd16[6] = 22;
  binStart16[7] = 23;
  binEnd16[7] = 32;
  binStart16[8] = 33;
  binEnd16[8] = 46;
  binStart16[9] = 47;
  binEnd16[9] = 66;
  binStart16[10] = 67;
  binEnd16[10] = 94;
  binStart16[11] = 95;
  binEnd16[11] = 133;
  binStart16[12] = 134;
  binEnd16[12] = 187;
  binStart16[13] = 188;
  binEnd16[13] = 262;
  binStart16[14] = 263;
  binEnd16[14] = 366;
  binStart16[15] = 367;
  binEnd16[15] = 511;

  // frequency range for the 25 vertical bars
  binStart25[0] = 0;
  binEnd25[0] = 0;
  binStart25[1] = 1;
  binEnd25[1] = 1;
  binStart25[2] = 2;
  binEnd25[2] = 2;
  binStart25[3] = 3;
  binEnd25[3] = 4;
  binStart25[4] = 5;
  binEnd25[4] = 6;
  binStart25[5] = 7;
  binEnd25[5] = 9;
  binStart25[6] = 10;
  binEnd25[6] = 12;
  binStart25[7] = 13;
  binEnd25[7] = 16;
  binStart25[8] = 17;
  binEnd25[8] = 20;
  binStart25[9] = 21;
  binEnd25[9] = 25;
  binStart25[10] = 26;
  binEnd25[10] = 31;
  binStart25[11] = 32;
  binEnd25[11] = 39;
  binStart25[12] = 40;
  binEnd25[12] = 48;
  binStart25[13] = 49;
  binEnd25[13] = 59;
  binStart25[14] = 60;
  binEnd25[14] = 73;
  binStart25[15] = 74;
  binEnd25[15] = 89;
  binStart25[16] = 90;
  binEnd25[16] = 109;
  binStart25[17] = 110;
  binEnd25[17] = 133;
  binStart25[18] = 134;
  binEnd25[18] = 162;
  binStart25[19] = 163;
  binEnd25[19] = 197;
  binStart25[20] = 198;
  binEnd25[20] = 239;
  binStart25[21] = 240;
  binEnd25[21] = 289;
  binStart25[22] = 290;
  binEnd25[22] = 350;
  binStart25[23] = 351;
  binEnd25[23] = 423;
  binStart25[24] = 424;
  binEnd25[24] = 511;

  // frequency range for the 52 horizontal bars
  binStart52[0] = 0;
  binEnd52[0] = 0;
  binStart52[1] = 1;
  binEnd52[1] = 1;
  binStart52[2] = 2;
  binEnd52[2] = 2;
  binStart52[3] = 3;
  binEnd52[3] = 3;
  binStart52[4] = 4;
  binEnd52[4] = 4;
  binStart52[5] = 5;
  binEnd52[5] = 5;
  binStart52[6] = 6;
  binEnd52[6] = 7;
  binStart52[7] = 8;
  binEnd52[7] = 9;
  binStart52[8] = 10;
  binEnd52[8] = 11;
  binStart52[9] = 12;
  binEnd52[9] = 13;
  binStart52[10] = 14;
  binEnd52[10] = 15;
  binStart52[11] = 16;
  binEnd52[11] = 17;
  binStart52[12] = 18;
  binEnd52[12] = 19;
  binStart52[13] = 20;
  binEnd52[13] = 21;
  binStart52[14] = 22;
  binEnd52[14] = 24;
  binStart52[15] = 25;
  binEnd52[15] = 27;
  binStart52[16] = 28;
  binEnd52[16] = 30;
  binStart52[17] = 31;
  binEnd52[17] = 33;
  binStart52[18] = 34;
  binEnd52[18] = 37;
  binStart52[19] = 38;
  binEnd52[19] = 41;
  binStart52[20] = 42;
  binEnd52[20] = 45;
  binStart52[21] = 46;
  binEnd52[21] = 49;
  binStart52[22] = 50;
  binEnd52[22] = 54;
  binStart52[23] = 55;
  binEnd52[23] = 59;
  binStart52[24] = 60;
  binEnd52[24] = 64;
  binStart52[25] = 65;
  binEnd52[25] = 70;
  binStart52[26] = 71;
  binEnd52[26] = 76;
  binStart52[27] = 77;
  binEnd52[27] = 83;
  binStart52[28] = 84;
  binEnd52[28] = 90;
  binStart52[29] = 91;
  binEnd52[29] = 98;
  binStart52[30] = 99;
  binEnd52[30] = 106;
  binStart52[31] = 107;
  binEnd52[31] = 115;
  binStart52[32] = 116;
  binEnd52[32] = 124;
  binStart52[33] = 125;
  binEnd52[33] = 134;
  binStart52[34] = 135;
  binEnd52[34] = 145;
  binStart52[35] = 146;
  binEnd52[35] = 157;
  binStart52[36] = 158;
  binEnd52[36] = 169;
  binStart52[37] = 170;
  binEnd52[37] = 182;
  binStart52[38] = 183;
  binEnd52[38] = 196;
  binStart52[39] = 197;
  binEnd52[39] = 211;
  binStart52[40] = 212;
  binEnd52[40] = 228;
  binStart52[41] = 229;
  binEnd52[41] = 246;
  binStart52[42] = 247;
  binEnd52[42] = 265;
  binStart52[43] = 266;
  binEnd52[43] = 285;
  binStart52[44] = 286;
  binEnd52[44] = 307;
  binStart52[45] = 308;
  binEnd52[45] = 330;
  binStart52[46] = 331;
  binEnd52[46] = 355;
  binStart52[47] = 356;
  binEnd52[47] = 382;
  binStart52[48] = 383;
  binEnd52[48] = 411;
  binStart52[49] = 412;
  binEnd52[49] = 442;
  binStart52[50] = 443;
  binEnd52[50] = 475;
  binStart52[51] = 476;
  binEnd52[51] = 511;

  // frequency range for the 69 vertical bars
  binStart69[0] = 0;
  binEnd69[0] = 0;
  binStart69[1] = 1;
  binEnd69[1] = 1;
  binStart69[2] = 2;
  binEnd69[2] = 2;
  binStart69[3] = 3;
  binEnd69[3] = 3;
  binStart69[4] = 4;
  binEnd69[4] = 4;
  binStart69[5] = 5;
  binEnd69[5] = 5;
  binStart69[6] = 6;
  binEnd69[6] = 6;
  binStart69[7] = 7;
  binEnd69[7] = 7;
  binStart69[8] = 8;
  binEnd69[8] = 8;
  binStart69[9] = 9;
  binEnd69[9] = 10;
  binStart69[10] = 11;
  binEnd69[10] = 12;
  binStart69[11] = 13;
  binEnd69[11] = 14;
  binStart69[12] = 15;
  binEnd69[12] = 16;
  binStart69[13] = 17;
  binEnd69[13] = 18;
  binStart69[14] = 19;
  binEnd69[14] = 20;
  binStart69[15] = 21;
  binEnd69[15] = 22;
  binStart69[16] = 23;
  binEnd69[16] = 24;
  binStart69[17] = 25;
  binEnd69[17] = 26;
  binStart69[18] = 27;
  binEnd69[18] = 28;
  binStart69[19] = 29;
  binEnd69[19] = 30;
  binStart69[20] = 31;
  binEnd69[20] = 33;
  binStart69[21] = 34;
  binEnd69[21] = 36;
  binStart69[22] = 37;
  binEnd69[22] = 39;
  binStart69[23] = 40;
  binEnd69[23] = 42;
  binStart69[24] = 43;
  binEnd69[24] = 45;
  binStart69[25] = 46;
  binEnd69[25] = 48;
  binStart69[26] = 49;
  binEnd69[26] = 51;
  binStart69[27] = 52;
  binEnd69[27] = 55;
  binStart69[28] = 56;
  binEnd69[28] = 59;
  binStart69[29] = 60;
  binEnd69[29] = 63;
  binStart69[30] = 64;
  binEnd69[30] = 67;
  binStart69[31] = 68;
  binEnd69[31] = 71;
  binStart69[32] = 72;
  binEnd69[32] = 76;
  binStart69[33] = 77;
  binEnd69[33] = 81;
  binStart69[34] = 82;
  binEnd69[34] = 86;
  binStart69[35] = 87;
  binEnd69[35] = 91;
  binStart69[36] = 92;
  binEnd69[36] = 96;
  binStart69[37] = 97;
  binEnd69[37] = 102;
  binStart69[38] = 103;
  binEnd69[38] = 108;
  binStart69[39] = 109;
  binEnd69[39] = 114;
  binStart69[40] = 115;
  binEnd69[40] = 121;
  binStart69[41] = 122;
  binEnd69[41] = 128;
  binStart69[42] = 129;
  binEnd69[42] = 135;
  binStart69[43] = 136;
  binEnd69[43] = 143;
  binStart69[44] = 144;
  binEnd69[44] = 151;
  binStart69[45] = 152;
  binEnd69[45] = 159;
  binStart69[46] = 160;
  binEnd69[46] = 168;
  binStart69[47] = 169;
  binEnd69[47] = 177;
  binStart69[48] = 178;
  binEnd69[48] = 187;
  binStart69[49] = 188;
  binEnd69[49] = 197;
  binStart69[50] = 198;
  binEnd69[50] = 208;
  binStart69[51] = 209;
  binEnd69[51] = 219;
  binStart69[52] = 220;
  binEnd69[52] = 231;
  binStart69[53] = 232;
  binEnd69[53] = 243;
  binStart69[54] = 244;
  binEnd69[54] = 256;
  binStart69[55] = 257;
  binEnd69[55] = 269;
  binStart69[56] = 270;
  binEnd69[56] = 283;
  binStart69[57] = 284;
  binEnd69[57] = 298;
  binStart69[58] = 299;
  binEnd69[58] = 313;
  binStart69[59] = 314;
  binEnd69[59] = 329;
  binStart69[60] = 330;
  binEnd69[60] = 346;
  binStart69[61] = 347;
  binEnd69[61] = 364;
  binStart69[62] = 365;
  binEnd69[62] = 382;
  binStart69[63] = 383;
  binEnd69[63] = 401;
  binStart69[64] = 402;
  binEnd69[64] = 421;
  binStart69[65] = 422;
  binEnd69[65] = 442;
  binStart69[66] = 443;
  binEnd69[66] = 464;
  binStart69[67] = 465;
  binEnd69[67] = 487;
  binStart69[68] = 488;
  binEnd69[68] = 511;
}
