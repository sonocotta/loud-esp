#pragma once

#include <TFT_eSPI.h>
#include "images/analog_black.h"
#include "images/analog_white.h"
#include "images/analog_warm.h"

class Visualiser
{
public:
    Visualiser(TFT_eSPI *tft)
        : tft(tft){};

    virtual void init(void);
    virtual void draw(int16_t valueLeft, int16_t valueRight);

protected:
    TFT_eSPI *tft;

    uint16_t rainbow(byte value);
    uint16_t getBackgroundColor(uint16_t _mode);

    float rmsAmplitudeTodB(int16_t amplitude, int16_t lr /*0=left, 1=right*/);
    float peakAmplitudeTodB(int16_t amplitude, int16_t lr /*0=left, 1=right*/, float peakAttenuation);
    float dBToRAD(float dB, float dB_min, float dB_max);
    float fftAmplitudeTodB(float amplitude, float minDb = -40.0f);

    // dB
    float dbLow = -45.0f;
    float dBHigh = 6.1f;
    float dBRMSL = -100.0f;
    float dBRMSR = -100.0f;
    float dBPPML = -100.0f;
    float dBPPMR = -100.0f;

    // calibration
    int RMSCorrectionLeft = 0;
    int RMSCorrectionRight = 0;
    int PPMCorrectionLeft = 0;
    int PPMCorrectionRight = 0;
};

void Visualiser::init(void) {}
void Visualiser::draw(int16_t valueLeft, int16_t valueRight) {}

/// <summary>
/// if 'value' is in the range 0-159 it is converted to a spectrum colour
/// from 0 = red through to 127 = blue to 159 = violet
/// extending the range to 0-191 adds a further violet to red band
/// </summary>
/// <param name="value"></param>
/// <returns>return a 16 bit rainbow colour</returns>
uint16_t Visualiser::rainbow(byte value)
{
    value = value % 192;

    byte red = 0;   // red is the top 5 bits of a 16 bit colour value
    byte green = 0; // green is the middle 6 bits, but only top 5 bits used here
    byte blue = 0;  // blue is the bottom 5 bits

    byte sector = value >> 5;
    byte amplit = value & 0x1F;

    switch (sector)
    {
    case 0:
        red = 0x1F;
        green = amplit; // green ramps up
        blue = 0;
        break;
    case 1:
        red = 0x1F - amplit; // red ramps down
        green = 0x1F;
        blue = 0;
        break;
    case 2:
        red = 0;
        green = 0x1F;
        blue = amplit; // blue ramps up
        break;
    case 3:
        red = 0;
        green = 0x1F - amplit; // green ramps down
        blue = 0x1F;
        break;
    case 4:
        red = amplit; // red ramps up
        green = 0;
        blue = 0x1F;
        break;
    case 5:
        red = 0x1F;
        green = 0;
        blue = 0x1F - amplit; // blue ramps down
        break;
    }

    return red << 11 | green << 6 | blue;
}

/// <summary>
/// get color from background
/// </summary>
/// <param name="_mode"></param>
/// <returns>background color</returns>
uint16_t Visualiser::getBackgroundColor(uint16_t _mode)
{
  uint16_t color = 0;
//   if (_mode == 0)
//     color = analog_black[119 + 160 * 240];
//   if (_mode == 1)
//     color = analog_white[119 + 160 * 240];
//   if (_mode == 2)
//     color = analog_warm[119 + 160 * 240];
  return color;
}

/// <summary>
/// converts RMS amplitude to dB plus delimiter
/// </summary>
/// <param name="amplitude"></param>
/// <param name="lr"></param>
/// <returns></returns>
float Visualiser::rmsAmplitudeTodB(int16_t amplitude, int16_t lr /*0=left, 1=right*/)
{
    if (amplitude == 0)
        return dbLow;

    float dB = 20.0f * log10f(abs(amplitude) * 2.0f / 32767.0) + 6.0f;

    if (lr == 0)
    {
        dB += RMSCorrectionLeft / 100.0f; // left
        dBRMSL = dB;
    }
    if (lr == 1)
    {
        dB += RMSCorrectionRight / 100.0f; // right
        dBRMSR = dB;
    }

    if (dB > dBHigh)
        dB = dBHigh;
    return dB;
}

/// <summary>
/// converts PEAK amplitude to dB plus delimiter, substract 'PeakAttenuation'
/// </summary>
/// <param name="amplitude"></param>
/// <param name="lr"></param>
/// <returns></returns>
float Visualiser::peakAmplitudeTodB(int16_t amplitude, int16_t lr /*0=left, 1=right*/, float peakAttenuation)
{
    if (amplitude == 0)
        return dbLow;

    float dB = 20.0f * log10f(abs(amplitude) * 2.0f / 32767.0) + 3.0f;
    dB -= peakAttenuation;

    if (lr == 0)
    {
        dB += PPMCorrectionLeft / 100.0f; // left
        dBPPML = dB;
    }
    if (lr == 1)
    {
        dB += PPMCorrectionRight / 100.0f; // light
        dBPPMR = dB;
    }

    if (dB > dBHigh)
        dB = dBHigh;
    return dB;
}

/// <summary>
/// converts dB to RAD plus delimiter
/// </summary>
/// <param name="dB"></param>
/// <param name="dB_min"></param>
/// <param name="dB_max"></param>
/// <returns>RAD</returns>
float Visualiser::dBToRAD(float dB, float dB_min, float dB_max)
{
  if (dB < dB_min)
    dB = dB_min;
  if (dB > dB_max)
    dB = dB_max;

  // range possible from -40dB to 7dB
  float rad = 0.00000015881455f * (dB * dB * dB * dB * dB) + 0.000014316862f * (dB * dB * dB * dB) + 0.00039325826f * (dB * dB * dB) + 0.0017871f * (dB * dB) - 0.0746641f * dB + 1.2338462f;

  if (rad < 1.0f)
    rad = 1.0f;
  if (rad > 2.15f)
    rad = 2.15f;

  return rad;
}


/// <summary>
/// converts FFT amplitude values to dB scale plus delimiter
/// </summary>
/// <param name="amplitude"></param>
/// <returns></returns>
float Visualiser::fftAmplitudeTodB(float amplitude, float minDb)
{
  if (amplitude < 0.01f)
    return minDb;
  float dB = 10.0f * log10f(amplitude / 0.775f);
  if (dB > dBHigh)
    dB = dBHigh;
  return dB;
}