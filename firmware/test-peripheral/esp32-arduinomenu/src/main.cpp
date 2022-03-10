#include <Arduino.h>
#include <TFT_eSPI.h>
#include <menu.h>
#include <menuIO/TFT_eSPIOut.h>
#include <streamFlow.h>
#include <ClickEncoder.h>
// Using this library: https://github.com/soligen2010/encoder.git
#include <menuIO/clickEncoderIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialIO.h>
// For debugging the TFT_eSPIOut.h library
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <menuIO/analogAxisIn.h>

// define menu colors --------------------------------------------------------
#define Black RGB565(0, 0, 0)
#define Red RGB565(255, 0, 0)
#define Green RGB565(0, 255, 0)
#define Blue RGB565(0, 0, 255)
#define Gray RGB565(128, 128, 128)
#define LighterRed RGB565(255, 150, 150)
#define LighterGreen RGB565(150, 255, 150)
#define LighterBlue RGB565(150, 150, 255)
#define LighterGray RGB565(211, 211, 211)
#define DarkerRed RGB565(150, 0, 0)
#define DarkerGreen RGB565(0, 150, 0)
#define DarkerBlue RGB565(0, 0, 150)
#define Cyan RGB565(0, 255, 255)
#define Magenta RGB565(255, 0, 255)
#define Yellow RGB565(255, 255, 0)
#define White RGB565(255, 255, 255)
#define DarkerOrange RGB565(255, 140, 0)

// TFT color table
const colorDef<uint16_t> colors[6] MEMMODE = {
    //{{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
    {{(uint16_t)Black, (uint16_t)Black}, {(uint16_t)Black, (uint16_t)Red, (uint16_t)Red}},     // bgColor
    {{(uint16_t)White, (uint16_t)White}, {(uint16_t)White, (uint16_t)White, (uint16_t)White}}, // fgColor
    {{(uint16_t)Red, (uint16_t)Red}, {(uint16_t)Yellow, (uint16_t)Yellow, (uint16_t)Yellow}},  // valColor
    {{(uint16_t)White, (uint16_t)White}, {(uint16_t)White, (uint16_t)White, (uint16_t)White}}, // unitColor
    {{(uint16_t)White, (uint16_t)Gray}, {(uint16_t)Black, (uint16_t)Red, (uint16_t)White}},    // cursorColor
    {{(uint16_t)White, (uint16_t)Yellow}, {(uint16_t)Black, (uint16_t)Red, (uint16_t)Red}},    // titleColor
};

using namespace Menu;

#ifdef TEST_ROTARY_ENC
// Declare the clickencoder
// Disable doubleclicks in setup makes the response faster.  See: https://github.com/soligen2010/encoder/issues/6
ClickEncoder clickEncoder = ClickEncoder(TEST_ROTARY_ENC_A, TEST_ROTARY_ENC_B, TEST_ROTARY_ENC, 4);
ClickEncoderStream encStream(clickEncoder, 1);
void IRAM_ATTR onTimer(); // Start the timer to read the clickEncoder every 1 ms
#endif

#ifdef TEST_JOYSTICK
// analog joystick parameters
//  uint8_t pin,
//  uint8_t sensivity=5,
//  bool inv=false,
//  int center=512,
//  int inertia=6,
//  int bufferSz=1,
//  navCmds posCmd=upCmd,
//  navCmds negCmd=downCmd
analogAxis<TEST_JOYSTICK_X, 5, false, 2048, 10, 1, escCmd, enterCmd> ax;
analogAxis<TEST_JOYSTICK_Y, 5, false, 2048, 10> ay;

keyMap JoystickButtonsMap[] = {{TEST_JOYSTICK_BTN, defaultNavCodes[enterCmd].ch}};
keyIn<1> JoystickButtons(JoystickButtonsMap); // 1 is the number of keys
#endif

int chooseField = 1;
int cutsMade = 0;
int numberOfCuts = 5;
int lengthOfCuts = 50; // Length in mm
int feedLength = 304;
int exitMenuOptions = 0; // Forces the menu to exit and cut the copper tape

// TFT gfx is what the ArduinoMenu TFT_eSPIOut.h is expecting
TFT_eSPI gfx = TFT_eSPI();

void feedInOut(); // Function to manually feed the tape without cutting
void runCuts();   // Main loop that feeds tape and calls servoCuts()

//////////////////////////////////////////////////////////
// Start ArduinoMenu
//////////////////////////////////////////////////////////

result doFeed()
{
  delay(500);
  exitMenuOptions = 2;
  return proceed;
}

result doRunCuts()
{
  delay(500);
  exitMenuOptions = 1;
  return proceed;
}

result updateEEPROM()
{
  // writeEEPROM();
  return quit;
}

#define MAX_DEPTH 3

MENU(subMenuAdjustServo, "Adjust Servo Settings", doNothing, noEvent, noStyle
     // ,FIELD(settingsEEPROM.servoOpen, "Servo Open", " degrees", 0, 180, 10, 1, doNothing, noEvent, noStyle)
     // ,FIELD(settingsEEPROM.servoClosed, "Servo Closed", " degrees", 0, 180, 10, 1, doNothing, noEvent, noStyle)
     // ,SUBMENU(subMenu)
     ,
     OP("Run!", doFeed, enterEvent), EXIT("<Back"));

CHOOSE(chooseField, feedDirChoose, "Choose Direction:", doNothing, noEvent, noStyle, VALUE("Forward", 1, doNothing, noEvent), VALUE("Backwards", 0, doNothing, noEvent));

MENU(subMenuFeedInOut, "Feed Tape", doNothing, noEvent, noStyle, FIELD(feedLength, "Length of Feed:", "mm", 0, 1000, 10, 1, doNothing, noEvent, noStyle), SUBMENU(feedDirChoose), OP("Run!", doFeed, enterEvent), EXIT("<Back"));

MENU(mainMenu, "COPPER TAPE CUTTER", doNothing, noEvent, wrapStyle, FIELD(lengthOfCuts, "Cut Size:", "mm", 0, 2000, 10, 1, doNothing, noEvent, noStyle), FIELD(numberOfCuts, "Pieces:", "", 0, 1000, 10, 1, doNothing, noEvent, noStyle), OP("Cut!", doRunCuts, enterEvent), SUBMENU(subMenuFeedInOut), SUBMENU(subMenuAdjustServo));

#define FONT_SIZE 1

/*
// default glcd font
#define fontW (6 * FONT_SIZE)
#define fontH (9 * FONT_SIZE)
*/

// font No 2
#define fontW (8 * FONT_SIZE)
#define fontH (16 * FONT_SIZE)

/*
// FreeMono9pt7b
#define fontW (11 * FONT_SIZE)
#define fontH (18 * FONT_SIZE)
*/

const panel panels[] MEMMODE = {{0, 0, TFT_HEIGHT / fontW, TFT_WIDTH / fontH}}; // Main menu panel
navNode *nodes[sizeof(panels) / sizeof(panel)];                                 // navNodes to store navigation status
panelsList pList(panels, nodes, sizeof(panels) / sizeof(panel));                // a list of panels and nodes

// idx_t tops[MAX_DEPTH]={0,0}; // store cursor positions for each level
idx_t eSpiTops[MAX_DEPTH] = {0};
TFT_eSPIOut eSpiOut(gfx, colors, eSpiTops, pList, fontW, fontH + 1);

idx_t serialTops[MAX_DEPTH] = {0};
serialOut outSerial(Serial, serialTops);

menuOut *constMEM outputs[] MEMMODE = {&outSerial, &eSpiOut};  // list of output devices
outputsList out(outputs, sizeof(outputs) / sizeof(menuOut *)); // outputs list

serialIn serial(Serial);
#ifdef TEST_ROTARY_ENC
MENU_INPUTS(in, &encStream, &serial);
#endif
#ifdef TEST_JOYSTICK
MENU_INPUTS(in, &ay, &ax, &JoystickButtons, &serial);
#endif
NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

// ESP32 timer thanks to: http://www.iotsharing.com/2017/06/how-to-use-interrupt-timer-in-arduino-esp32.html
// and: https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//////////////////////////////////////////////////////////
// End Arduino Menu
//////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(SERIAL_BAUD);

#ifdef TEST_ROTARY_ENC
  clickEncoder.setAccelerationEnabled(true);
  clickEncoder.setDoubleClickEnabled(false); // Disable doubleclicks makes the response faster.  See: https://github.com/soligen2010/encoder/issues/6

  // ESP32 timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
#endif

#ifdef TEST_JOYSTICK
  JoystickButtons.begin();
#endif

  // Use this initializer if you're using a 1.8" TFT
  // SPI.begin();
  gfx.init();
  gfx.setRotation(TFT_ROTATION);
  Serial.println("Initialized TFT");
  gfx.fillScreen(TFT_BLACK);
  // gfx.setFreeFont(&FreeMono9pt7b);
  gfx.setTextFont(2);
  gfx.setTextSize(FONT_SIZE);
  Serial.println("done");

  nav.showTitle = true; // Show titles in the menus and submenus
  //  nav.timeOut = 60;  // Timeout after 60 seconds of inactivity
  //  nav.idleOn(); // Start with the main screen and not the menu
}

void loop()
{
  // Slow down the menu redraw rate
  constexpr int menuFPS = 1000 / 30;
  static unsigned long lastMenuFrame = -menuFPS;
  unsigned long now = millis();
  //... other stuff on loop, will keep executing
  switch (exitMenuOptions)
  {
  case 1:
  {
    delay(500); // Pause to allow the button to come up
    runCuts();
    break;
  }
  case 2:
  {
    delay(500); // Pause to allow the button to come up
    feedInOut();
    break;
  }
  default: // Do the normal program functions with ArduinoMenu
    if (now - lastMenuFrame >= menuFPS)
    {
      lastMenuFrame = millis();
      nav.poll(); // Poll the input devices
    }
  }
}

void runCuts()
{
  exitMenuOptions = 0; // Return to the menu
  delay(500);
  mainMenu.dirty = true; // Force the main menu to redraw itself
}

void feedInOut()
{
  exitMenuOptions = 0; // Return to the menu
  delay(500);
  mainMenu.dirty = true; // Force the main menu to redraw itself
}

#ifdef TEST_ROTARY_ENC
// ESP32 timer
void IRAM_ATTR onTimer()
{
  clickEncoder.service();
}
#endif