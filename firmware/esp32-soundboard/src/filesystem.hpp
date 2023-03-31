#pragma once

#include <SD.h>
#include <AudioFileSourceSD.h>
#define AudioFileSource AudioFileSourceSD
#define FileSystemClass SD

#define MAX_FILES_COUNT (12 * 3)

struct FilePackage
{
    String name;
    // AudioFileSource *file;
    String audio;
    // AudioGenerator *generator;
    String image;
};

class FileSystem
{
private:
    uint8_t folders_count = 0;
    FilePackage audio_files[MAX_FILES_COUNT];

public:
    FileSystem(){};

    bool begin();
    void loadFilesList();
    FilePackage getFileByIndex(uint8_t ix);
};

bool FileSystem::begin()
{
    return FileSystemClass.begin(PIN_SD_CS,
        SPI, 4000000, "/sd", MAX_FILES_COUNT);
}

void FileSystem::loadFilesList()
{
    File root = FileSystemClass.open("/", FILE_READ);
    uint8_t dir_counter = 0;

    while (File rootdir = root.openNextFile())
    {
        String rootdirName = rootdir.name();
        if (rootdir.isDirectory())
        {
            log_i("Found dir: %s", rootdirName);
            FilePackage thisFolder = {
                .name = ""
            };

            while (File infile = rootdir.openNextFile())
            {
                if (!infile.isDirectory())
                {
                    String fileName = infile.name();
                    log_i("\t%s : %d KB", infile.name(), infile.size() >> 10);
                    if (fileName.endsWith(".txt")) {
                        thisFolder.name = fileName.substring(0, fileName.length() - 4);
                    }
                    if (fileName.endsWith(".bmp")) {
                        thisFolder.image = "/" + rootdirName + "/" + fileName;
                    }
                    if (fileName.endsWith(".wav")) {
                        //thisFolder.file = new AudioFileSource(("/" + rootdirName + "/" + fileName).c_str());
                        thisFolder.audio = "/" + rootdirName + "/" + fileName;
                    }
                }
            }

            if (thisFolder.name != "") {
                audio_files[dir_counter++] = thisFolder;
            }
        }
    }

    folders_count = dir_counter;
    log_d("%d folders found", folders_count);
}

FilePackage FileSystem::getFileByIndex(uint8_t ix)
{
    return audio_files[ix];
}