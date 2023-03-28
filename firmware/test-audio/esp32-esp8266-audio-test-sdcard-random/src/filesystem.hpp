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

struct FilePackage {
    String fileName;
    AudioFileSource *file;
    AudioGenerator *generator;
};

class FileSystem
{
private:
    uint8_t audio_files_count = 0;
    FilePackage audio_files[MAX_FILES_COUNT];
    AudioGenerator* getAudioGenerator(String fileName);

public:
    FileSystem(){};

    bool begin();
    void loadFilesList();
    FilePackage getFileByRandom();
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

AudioGenerator* FileSystem::getAudioGenerator(String fileName)
{
  if (fileName.endsWith(".ogg"))
  {
    return new AudioGeneratorOpus();
  }
  else if (fileName.endsWith(".wav"))
  {
    return new AudioGeneratorWAV();
  }
  else if (fileName.endsWith(".mp3"))
  {
    return new AudioGeneratorMP3();
  }
  else
  {
    return nullptr;
  }
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
            AudioGenerator* gen = getAudioGenerator(fileName);
            if (gen != nullptr) {
                audio_files[files_counter] = {
                    .fileName = fileName,
                    .file = new AudioFileSource((
                    #ifdef ESP32
                        "/" +
                    #endif
                        fileName).c_str()),
                    .generator = getAudioGenerator(fileName)
                };
                files_counter++;
            } else {
                Serial.printf("\t\tUnknown extenstion, skipping\n");
            }
        }

        audio_files_count = files_counter;
}

FilePackage FileSystem::getFileByRandom()
{
  uint8_t index = random(audio_files_count);
  return audio_files[index];
}