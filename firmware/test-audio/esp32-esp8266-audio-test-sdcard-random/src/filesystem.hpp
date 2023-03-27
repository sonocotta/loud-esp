#pragma once

#ifdef USE_SPIFFS
#ifdef ESP32
#include "SPIFFS.h"
#endif
#include <AudioFileSourceSPIFFS.h>
#define AudioFileSource AudioFileSourceSPIFFS
#define FileSystemClass SPIFFS
#endif

#ifdef USE_SD
#include <SD.h>
#include <AudioFileSourceSD.h>
#define AudioFileSource AudioFileSourceSD
#define FileSystemClass SD
#endif

#define MAX_FILES_COUNT 16

class FileSystem
{
private:
    uint8_t audio_files_count = 0;
    String audio_files[MAX_FILES_COUNT];

public:
    FileSystem(){};

    bool begin();
    void loadFilesList();
    AudioFileSource *getFileByIndex(uint8_t index);
};

bool FileSystem::begin()
{
#ifdef USE_SPIFFS
    return FileSystemClass.begin();
#endif

#ifdef USE_SD
    return FileSystemClass.begin(PIN_SD_CS);
#endif
}

void FileSystem::loadFilesList()
{

// #ifdef USE_SPIFFS
// #ifdef ESP8266
// #endif
#if defined(ESP32) || defined(USE_SD)
    File dir = FileSystemClass.open("/", FILE_READ);
#else
    Dir dir = FileSystemClass.openDir("/");
#endif
// #endif

// #ifdef USE_SD
//     File dir = FileSystemClass.open("/");
// #endif

   uint8_t files_counter = 0;
#if defined(ESP32) || defined(USE_SD)
    while (File file = dir.openNextFile())
#else
    while (dir.next())
#endif
        {
#if defined(ESP32) || defined(USE_SD)
            String fileName = file.name();
            uint32_t fileSize = file.size();
#else
            String fileName = dir.fileName();
            uint32_t fileSize = dir.fileSize();
#endif
            Serial.printf("\t%s : %d bytes\n", fileName.c_str(), fileSize);
            audio_files[files_counter] = fileName;
            files_counter++;
        }

        audio_files_count = files_counter;
}

AudioFileSource* FileSystem::getFileByIndex(uint8_t index)
{
  String fileName = String(audio_files[index % audio_files_count]);
  return new AudioFileSource((
#ifdef ESP32
    "/" +
#endif
    fileName).c_str());
}