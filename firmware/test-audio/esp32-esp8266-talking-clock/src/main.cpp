#include <Arduino.h>

#ifdef USE_TFT
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();
#endif

#ifdef ESP32
#include <WiFi.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif
#include <WiFiManager.h>
WiFiManager _wifiManager;

#include <Ticker.h>
Ticker ticker0, ticker1;

#include <time.h>
#include "tz.h"
struct tm tm; // http://www.cplusplus.com/reference/ctime/tm/

#define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
#define NTP_MIN_VALID_EPOCH 1533081600 // August 1st, 2018

String M_arr[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String WD_arr[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// extern "C" uint8_t sntp_getreachability(uint8_t);

#include <AudioOutputI2S.h>
#include <AudioOutputMixer.h>
#include <ESPSAM.h>
AudioOutputI2S *hwout = new AudioOutputI2S();
AudioOutputMixer *out;
AudioOutputMixerStub *stub[1];
ESPSAM *sam = new ESPSAM;

// #include "OneButton.h"
// OneButton button(PIN_BTN, true, false);
#ifdef USE_TFT
void status (String statusText, uint8_t fontNum = 4, uint16_t color = TFT_LIGHTGREY, uint8_t textDatum = MC_DATUM, uint16_t x = tft.width() / 3, uint16_t y = tft.height() / 2) 
{
    tft.fillScreen(TFT_BLACK);   
    tft.setCursor(x, y);
    tft.setTextDatum(textDatum);
    tft.setTextFont(fontNum);
    tft.setTextColor(color);
    tft.print(statusText);
}
#else
void status (String statusText) 
{
    Serial.println(statusText);
}
#endif

// connect WiFi -> fetch ntp packet -> disconnect Wifi
void syncTime()
{
    status("Connecting WiFi");

    WiFi.mode(WIFI_STA);
    // https://github.com/tzapu/WiFiManager/issues/1422
    #ifdef ARDUINO_LOLIN_C3_MINI
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    #endif
    
    if (!_wifiManager.autoConnect())
    {
        Serial.println("Failed to connect and hit timeout");
#ifdef ESP8266
        ESP.reset();
#endif
#ifdef ESP32
        ESP.restart();
#endif
    }
    else
    {
        Serial.println("\nConnected with: " + WiFi.SSID());
        Serial.println("IP Address: " + WiFi.localIP().toString());
    }

    time_t now;

    status("Requesting time");

#ifdef ESP8266
    configTime(TIMEZONE, NTP_SERVERS);
#endif
#ifdef ESP32
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVERS);
#endif

    Serial.print("Requesting current time ");
    int i = 1;
    while ((now = time(nullptr)) < NTP_MIN_VALID_EPOCH)
    {
        Serial.print(".");
        delay(300);
        yield();
        i++;
    }
    Serial.println("Time synchronized");
    Serial.printf("Local time: %s", asctime(localtime(&now))); // print formated local time, same as ctime(&now)
    Serial.printf("UTC time:   %s", asctime(gmtime(&now)));    // print formated GMT/UTC time

    WiFi.mode(WIFI_OFF);

    status("Ready");
}

void pronounceTime() {
    sam->Say(stub[0], "Time is ");
    sam->Say(stub[0], tm.tm_hour);
    sam->Say(stub[0], " hours and ");
    sam->Say(stub[0], tm.tm_min);
    sam->Say(stub[0], " minutes");
}

void setup()
{
    Serial.begin(SERIAL_BAUD);

    #ifdef USE_TFT
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);

    status("Starting");

    #endif

    out = new AudioOutputMixer(32, hwout);
    stub[0] = out->NewInput();
    stub[0]->SetGain(0.2);

    WiFi.mode(WIFI_OFF);

    String ssid = "ESP-" + String((unsigned long)
#ifdef ESP8266
    ESP.getChipId()
#endif
#ifdef ESP32
    ESP.getEfuseMac()
#endif
    );

    WiFi.hostname(ssid);

    syncTime();

    // button.attachClick([]() {
    //     Serial.println("Clicked");
    //     pronounceTime();
    // });

    // button.attachDoubleClick([]() {
    //     Serial.println("Double clicked");
    //     sam->Say(stub[0], "It is ");
    //     sam->Say(stub[0], WD_arr[tm.tm_wday].c_str());
    //     sam->Say(stub[0], " , ");
    //     sam->Say(stub[0], tm.tm_mday);
    //     sam->Say(stub[0], " of ");
    //     sam->Say(stub[0], M_arr[tm.tm_mon].c_str());
    // });
    
    ticker0.attach_ms(1000, []() {
        time_t now = time(&now);
        localtime_r(&now, &tm);

        char buffer[12];
        sprintf(buffer, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
        #ifdef USE_TFT
        status(buffer, 7, TFT_GREEN, BL_DATUM, tft.width() >> 3);
        #else
        status(buffer);
        #endif

        if (tm.tm_sec == 0)
            pronounceTime();
    });

    ticker1.attach_ms(5000, []() {
        Serial.printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    });

    hwout->begin();
}

void loop()
{
    // button.tick();
    // delay(10);
}