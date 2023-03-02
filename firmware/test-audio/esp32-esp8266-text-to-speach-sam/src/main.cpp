#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif
#include <SD.h>
#ifdef ESP32
#include "SPIFFS.h"
#endif

#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioOutputMixer.h"

const char *SSID = "wifi-12-private";
const char *PASSWORD = "9263777101";

const char *URL = "http://51.144.137.103:18000/an";

AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioFileSourceBuffer *buff;
AudioOutputI2S *out;
AudioOutputMixer *mixer;
AudioOutputMixerStub *stub;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void)isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2) - 1] = 0;
  Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
  Serial.flush();
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("Connecting to WiFi");

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" connected!");

#ifdef PIN_I2S_EN
  pinMode(PIN_I2S_EN, OUTPUT);
  digitalWrite(PIN_I2S_EN, HIGH);
#endif

  file = new AudioFileSourceHTTPStream(URL);
  file->RegisterMetadataCB(MDCallback, (void *)"ICY");
  buff = new AudioFileSourceBuffer(file, 8192);
  buff->RegisterStatusCB(StatusCallback, (void *)"buffer");
  out = new AudioOutputI2S();
  mixer = new AudioOutputMixer(32, out);
  stub = mixer->NewInput();
  stub->SetGain(0.2);
  mp3 = new AudioGeneratorMP3();
  mp3->RegisterStatusCB(StatusCallback, (void *)"mp3");

  Serial.println("Starting player:");
  Serial.println(URL);
  mp3->begin(buff, stub);
}

void loop()
{
  static int lastms = 0;

  if (mp3->isRunning())
  {
    if (millis() - lastms > 1000)
    {
      lastms = millis();
      Serial.printf("Running for %d ms...\n", lastms);
      Serial.flush();
    }
    if (!mp3->loop())
      mp3->stop();
  }
  else
  {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}