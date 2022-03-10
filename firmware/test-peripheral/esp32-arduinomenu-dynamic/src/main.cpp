/*
dynamic menu tests (experimental)

this is a somehow dynamic menu, its ok to use it on non AVR's
or with USE_RAM defined

the dynamic support is somehow limited
we do not support adding or removing options
however changing the list of options should be allowed

or replacing options

the user is responsible for all option allocation
deleting them if created with new, etc...

this can turn out to be a mess

objects construction is not realy tested beyond this example
please let me know if you have any interest on this kind of things

this is nice to support dynamic options like a list of detected wifi networks
or a list of files+folders from a folder/file system
*/

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
// TFT gfx is what the ArduinoMenu TFT_eSPIOut.h is expecting
TFT_eSPI gfx = TFT_eSPI();

using namespace Menu;
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define LED LED_BUILTIN

#ifndef USING_RAM
#error "This menu example does not work on flash memory versions (MENU_USEPGM)"
#error "Library must be compiled with MENU_USERAM defined (default for non AVR's)"
#error "ex: passing -DMENU_USERAM to the compiler"
#endif

// choose field and options -------------------------------------
int duration = 0; // target var
prompt *durData[] = {
    new menuValue<int>("Off", 0),
    new menuValue<int>("Short", 1),
    new menuValue<int>("Medium", 2),
    new menuValue<int>("Long", 3)};
choose<int> &durMenu = *new choose<int>("Duration", duration, sizeof(durData) / sizeof(prompt *), durData);

// select field and options -------------------------------------
enum Fxs
{
  Fx0,
  Fx1,
  Fx2
} selFx; // target var

prompt *fxData[] = {
    new menuValue<Fxs>("Default", Fx0),
    new menuValue<Fxs>("Pop", Fx1),
    new menuValue<Fxs>("Rock", Fx2)};

Menu::select<Fxs> &fxMenu = *new Menu::select<Fxs>("Fx", selFx, sizeof(fxData) / sizeof(prompt *), fxData);

// toggle field and options -------------------------------------
bool led = false; // target var
void setLed() { digitalWrite(LED, !led); }
prompt *togData[] = {
    new menuValue<bool>("On", true),
    new menuValue<bool>("Off", false)};
toggle<bool> &ledMenu = *new toggle<bool>("LED:", led, sizeof(togData) / sizeof(prompt *), togData, (Menu::callback)setLed, enterEvent);

// the submenu -------------------------------------
prompt *subData[] = {
    new prompt("Sub1"),
    new prompt("Sub2"),
    new Exit("<Back")};

menuNode &subMenu = *new menuNode("sub-menu", sizeof(subData) / sizeof(prompt *), subData);

// pad menu --------------------
uint16_t year = 2017;
uint16_t month = 10;
uint16_t day = 7;

prompt *padData[] = {
    new menuField<typeof(year)>(year, "", "", 1900, 3000, 20, 1, doNothing, noEvent),
    new menuField<typeof(month)>(month, "/", "", 1, 12, 1, 0, doNothing, noEvent),
    new menuField<typeof(day)>(day, "/", "", 1, 31, 1, 0, doNothing, noEvent)};

menuNode &padMenu = *new menuNode(
    "Date",
    sizeof(padData) / sizeof(prompt *),
    padData,
    doNothing,
    noEvent,
    noStyle,
    (systemStyles)(_asPad | Menu::_menuData | Menu::_canNav | _parentDraw));

// the main menu -------------------------------------
void op1Func() { Serial.println("Op 1 executed"); }
uint8_t test = 55; // target var for numerical range field

// edit text field info
const char *hexDigit MEMMODE = "0123456789ABCDEF";            // a text table
const char *hexNr[] MEMMODE = {"0", "x", hexDigit, hexDigit}; // text validators
char buf1[] = "0x11";                                         // text edit target

prompt *mainData[] = {
    new prompt("Op 1", (Menu::callback)op1Func, enterEvent),
    new prompt("Op 2"), // we can set/change text, function and event mask latter
    new menuField<typeof(test)>(test, "Bright", "", 0, 255, 10, 1, doNothing, noEvent),
    new textField("Addr", buf1, 4, (char *const *)hexNr),
    &subMenu,
    &durMenu,
    &fxMenu,
    &ledMenu,
    &padMenu,
    new Exit("<Exit.")};
    
menuNode &mainMenu = *new menuNode("Main menu", sizeof(mainData) / sizeof(prompt *), mainData /*,doNothing,noEvent,wrapStyle*/);

#define MAX_DEPTH 2

// input -------------------------------------
serialIn serial(Serial);
menuIn *inputsList[] = {&serial};
chainStream<1> ins(inputsList);


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

// serial output -------------------------------------
idx_t tops[MAX_DEPTH];
serialOut outSerial(Serial, tops);

// outputs -------------------------------------
menuOut *constMEM outList[] MEMMODE = {&outSerial, &eSpiOut};
outputsList outs(outList, sizeof(outList) / sizeof(menuOut *));

// navigation control -------------------------------------
navNode path[MAX_DEPTH];
navRoot nav(mainMenu, path, MAX_DEPTH, ins, outs);

void setup()
{
  pinMode(LED, OUTPUT);
  setLed();
  Serial.begin(115200);
  while (!Serial)     ;

  Serial.println("menu 4.x test");
  Serial.flush();

  gfx.init();
  gfx.setRotation(TFT_ROTATION);
  Serial.println("Initialized TFT");
  gfx.fillScreen(TFT_BLACK);
  // gfx.setFreeFont(&FreeMono9pt7b);
  gfx.setTextFont(2);
  gfx.setTextSize(FONT_SIZE);
  
  mainMenu[0].shadow->text = "Changed";
}

void loop()
{
  nav.poll(); // this device only draws when needed
  if (nav.sleepTask)
  {
    Serial.println();
    Serial.println("menu is suspended");
    Serial.println("presse [select] to resume.");
  }
  delay(100); // simulate a delay when other tasks are done
}