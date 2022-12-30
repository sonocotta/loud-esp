#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifdef TEST_TFT
#include "SPI.h"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft); // Sprite object for needle
TFT_eSprite spr = TFT_eSprite(&tft);    // Sprite for meter reading

#if (TFT_WIDTH == 240) && (TFT_HEIGHT == 240)
#include "sound-vu-meter-240x240.h"
#endif

#if (TFT_WIDTH == 240) && (TFT_HEIGHT == 320)
#include "sound-vu-meter-240x320.h"
#endif

#if (TFT_WIDTH == 320) && (TFT_HEIGHT == 240)
#include "sound-vu-meter-240x320.h"
#endif
#endif

#ifdef TEST_RGB
#include <FastLED.h>
#define NUM_LEDS 9
CRGB leds[NUM_LEDS];
#endif

#ifdef TEST_ROTARY_ENC
#include <ESP32Encoder.h>
ESP32Encoder encoder;
#endif

#define NEEDLE_LENGTH (TFT_WIDTH / 3) // Visible length
#define NEEDLE_TIP_LENGTH 10
#define NEEDLE_WIDTH 2                            // Width of needle - make it an odd number
#define NEEDLE_RADIUS (TFT_WIDTH - NEEDLE_LENGTH) // Radius at tip
#define NEEDLE_ANGLE_START -60
#define NEEDLE_ANGLE_MAX 110
#define NEEDLE_COLOR1 0b0000011100011000
#define NEEDLE_COLOR2 0b0000011100000000
#define DIAL_CENTRE_X (TFT_HEIGHT >> 1)
#define DIAL_CENTRE_Y (TFT_WIDTH)

// Font attached to this sketch
#include "NotoSansBold36.h"
#define AA_FONT_LARGE NotoSansBold36

uint16_t *tft_buffer;
bool buffer_loaded = false;
uint16_t spr_width = 0;
uint32_t start_x, start_y;

void createNeedle(void);
void plotNeedle(int16_t angle, uint16_t ms_delay);

// =======================================================================================
// Setup
// =======================================================================================
void setup()
{
  Serial.begin(SERIAL_BAUD);

#ifdef TEST_ROTARY_ENC
  pinMode(TEST_ROTARY_ENC, INPUT);
  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachHalfQuad(TEST_ROTARY_ENC_A, TEST_ROTARY_ENC_B);
#endif

#ifdef TEST_JOYSTICK
  pinMode(TEST_JOYSTICK_BTN, INPUT_PULLUP);
#endif

#ifdef TEST_RGB
  FastLED.addLeds<WS2812B, TEST_RGB, GRB>(leds, NUM_LEDS);

  leds[0] = CRGB::Red;
  FastLED.show();
  delay(250);
  leds[0] = CRGB::Black;
  FastLED.show();
#endif

  tft.begin();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);

#ifdef TFT_INVERT
  tft.invertDisplay(true);
#endif
  tft.setSwapBytes(true);

  // Draw the dial
  // TJpgDec.drawJpg(0, 0, dial, sizeof(dial));
  start_x = (tft.width() - Image.width) >> 1;
  start_y = (tft.height() - Image.height) >> 1;

  tft.pushImage(start_x, start_y, Image.width, Image.height, Image.data);

  // circle for debug
  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, NEEDLE_RADIUS - NEEDLE_LENGTH, TFT_DARKGREY);

  // Load the font and create the Sprite for reporting the value
  spr.loadFont(AA_FONT_LARGE);
  spr_width = spr.textWidth("277");
  spr.createSprite(spr_width, spr.fontHeight());
  uint16_t bg_color = tft.readPixel(DIAL_CENTRE_X, DIAL_CENTRE_Y); // Get colour from dial centre
  spr.fillSprite(bg_color);
  spr.setTextColor(TFT_WHITE, bg_color);
  spr.setTextDatum(MC_DATUM);
  spr.setTextPadding(spr_width);
  spr.drawNumber(0, spr_width / 2, spr.fontHeight() / 2);
  spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight());

  // Plot the label text
  // tft.setTextColor(TFT_WHITE, bg_color);
  // tft.setTextDatum(MC_DATUM);
  // tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

  // Define where the needle pivot point is on the TFT before
  // creating the needle so boundary calculation is correct
  tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);

  // Create the needle Sprite
  createNeedle();

  // Reset needle position to 0
  plotNeedle(0, 0);

  delay(2000);
}

// =======================================================================================
// Loop
// =======================================================================================

uint16_t angle = 0;
uint16_t saved_angle = 0;
uint8_t button_prev_state = 0;
int16_t encoder_prev_state = 0;

void loop()
{
#ifdef TEST_ROTARY_ENC
  auto encRead = encoder.getCount();
  if (encoder_prev_state != encRead)
  {
    Serial.printf("Encoder reads: %d\n", encRead);
    angle = constrain(-2 * encRead, 0, NEEDLE_ANGLE_MAX);
    encoder_prev_state = encRead;
  }

  auto btnRead = digitalRead(TEST_ROTARY_ENC);
  if (button_prev_state != btnRead)
  {
    Serial.printf("Encoder button reads: %s\n", btnRead == 1 ? "high" : "low");
    if (btnRead == HIGH)
    {
      saved_angle = angle;
      angle = NEEDLE_ANGLE_MAX;
    }
    else
    {
      angle = saved_angle;
    }
    button_prev_state = btnRead;
  }

#ifdef TEST_RGB
  uint8_t ledsCount = map(angle, 0, NEEDLE_ANGLE_MAX, 1, NUM_LEDS);
  uint8_t intensity = map(angle, 0, NEEDLE_ANGLE_MAX, 32, 255);
  auto color = (CRGB){intensity, 255 - intensity, 0};
  for (uint8_t i = 1; i < NUM_LEDS; i++)
    leds[i] = (i < ledsCount ? color : CRGB::Black);
  FastLED.show();
#endif
#endif

#ifdef TEST_JOYSTICK
  uint32_t x = analogRead(TEST_JOYSTICK_A) >> 2;
  uint32_t y = analogRead(TEST_JOYSTICK_B) >> 2;
  angle = map(1024 - x, 0, 1024, 0, NEEDLE_ANGLE_MAX);
  Serial.printf("Joystick readings: %d, %d, %d\n", y, x, digitalRead(TEST_JOYSTICK_BTN));

  #ifdef TEST_RGB
  uint8_t ledsCount = map(y, 0, 1023, 1, NUM_LEDS);
  uint8_t intensity = map(y, 0, 1023, 32, 255);
  auto color = (CRGB) {intensity, 255 - intensity, 0};
  for (uint8_t i = 1; i < NUM_LEDS; i++)
    leds[i] = (i < ledsCount ? color : CRGB::Black);
  FastLED.show();
  #endif

#endif

  // Serial.println(angle);

  plotNeedle(angle, 5);

  // Pause at new position
  delay(50);
}

// =======================================================================================
// Create the needle Sprite
// =======================================================================================
void createNeedle(void)
{
  needle.setColorDepth(8);
  needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH); // create the needle Sprite

  needle.fillSprite(TFT_BLACK); // Fill with black

  // Define needle pivot point relative to top left corner of Sprite
  uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)
  uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
  needle.setPivot(piv_x, piv_y);     // Set pivot point in this Sprite

  needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_TIP_LENGTH, NEEDLE_COLOR2);
  needle.fillRect(0, NEEDLE_TIP_LENGTH, NEEDLE_WIDTH, NEEDLE_LENGTH - NEEDLE_TIP_LENGTH, NEEDLE_COLOR1);

  // Bounding box parameters to be populated
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Work out the worst case area that must be grabbed from the TFT,
  // this is at a 45 degree rotation
  needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

  // Calculate the size and allocate the buffer for the grabbed TFT area
  tft_buffer = (uint16_t *)malloc(((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2);
}

// =======================================================================================
// Move the needle to a new position
// =======================================================================================
void plotNeedle(int16_t angle, uint16_t ms_delay)
{
  static int16_t old_angle = NEEDLE_ANGLE_START;

  // Bounding box parameters
  static int16_t min_x;
  static int16_t min_y;
  static int16_t max_x;
  static int16_t max_y;

  if (angle < 0)
    angle = 0; // Limit angle to emulate needle end stops
  if (angle > NEEDLE_ANGLE_MAX)
    angle = NEEDLE_ANGLE_MAX;

  angle += NEEDLE_ANGLE_START;

  // Move the needle until new angle reached
  while (angle != old_angle || !buffer_loaded)
  {

    if (old_angle < angle)
      old_angle++;
    else
      old_angle--;

    // Only plot needle at even values to improve plotting performance
    if ((old_angle & 1) == 0)
    {
      if (buffer_loaded)
      {
        // Paste back the original needle free image area
        tft.pushRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
      }

      if (needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y))
      {
        // Grab a copy of the area before needle is drawn
        tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
        buffer_loaded = true;
      }

      // Draw the needle in the new position, black in needle image is transparent
      needle.pushRotated(old_angle, TFT_BLACK);

      // Wait before next update
      delay(ms_delay);
    }

    // Update the number at the centre of the dial
    spr.drawNumber(old_angle - NEEDLE_ANGLE_START, spr_width / 2, spr.fontHeight() / 2);
    spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight());

    // Slow needle down slightly as it approaches the new position
    if (abs(old_angle - angle) < 10)
      ms_delay += ms_delay / 5;
  }
}