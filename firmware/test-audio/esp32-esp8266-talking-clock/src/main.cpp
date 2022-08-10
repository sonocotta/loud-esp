#include <Arduino.h>
//#include <String.h>

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
#include <ESPSAM.h>
AudioOutputI2S *out = new AudioOutputI2S();
ESPSAM *sam = new ESPSAM;

#include "OneButton.h"
OneButton button(PIN_BTN, true, false);

// connect WiFi -> fetch ntp packet -> disconnect Wifi
void syncTime()
{
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
}

void setup()
{
    Serial.begin(SERIAL_BAUD);

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

    button.attachClick([]() {
        sam->Say(out, "Time is ");
        sam->Say(out, tm.tm_hour);
        sam->Say(out, " hours and ");
        sam->Say(out, tm.tm_min);
        sam->Say(out, " minutes");
    });

    button.attachDoubleClick([]() {
        sam->Say(out, "It is ");
        sam->Say(out, WD_arr[tm.tm_wday].c_str());
        sam->Say(out, " , ");
        sam->Say(out, tm.tm_mday);
        sam->Say(out, " of ");
        sam->Say(out, M_arr[tm.tm_mon].c_str());
    });
    
    ticker0.attach_ms(1000, []() {
        time_t now = time(&now);
        localtime_r(&now, &tm);
    });

    ticker1.attach_ms(5000, []() {
        Serial.printf("%d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    });

    out->begin();
}

void loop()
{
    button.tick();
    delay(10);
}