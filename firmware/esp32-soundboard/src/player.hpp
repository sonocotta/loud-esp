#pragma once

#include <AudioOutputI2S.h>
#include <AudioGeneratorWAV.h>
#include "AudioOutputMixer.h"

#include "filesystem.hpp"

#define TILES_CNT (BRD_ROWS * BRD_COLS)

enum State
{
    INIT,
    PLAY,
    PAUSE,
    STOP,
};

const char *StateSymbols = "IPHS";

class Player
{
private:
    uint32_t state_changed_at = 0;
    AudioOutputI2S out;

    AudioGeneratorWAV *gen[TILES_CNT];
    AudioFileSourceSD *files[TILES_CNT];

    // default max stubs count is 8, you need to update it in library to support more
    // ESP8266Audio/src/AudioOutputMixer.h:75 enum { maxStubs = 16 }; 
    AudioOutputMixer mixer = AudioOutputMixer(64, &out);
    AudioOutputMixerStub *stubs[TILES_CNT];

    FileSystem *fs;

public:
    Player(FileSystem *fs)
        : fs(fs){};

    State states[TILES_CNT] = {INIT};

    void begin();
    void loop();
    void debug();
    void toggle(uint8_t ix);
};

void Player::begin()
{
    for (uint8_t ix = 0; ix < TILES_CNT; ix++)
    {
        auto file = fs->getFileByIndex(ix);
        log_i("Loading %s", file.audio.c_str());
        files[ix] = new AudioFileSourceSD(file.audio.c_str());
        stubs[ix] = mixer.NewInput();
        // stubs[ix]->SetGain(AUDIO_GAIN);
        gen[ix] = new AudioGeneratorWAV();
    }
}

void Player::toggle(uint8_t ix)
{
    switch (states[ix])
    {
    case INIT:
        #ifdef AUDIO_ONE_TRACK_ONLY
        for (uint8_t i = 0; i < TILES_CNT; i++)
            if (states[i] == PLAY) {
                log_d("Pausing track # %d", i);
                stubs[i]->stop();
                states[i] = PAUSE;
            }
        #endif
        log_d("Starting track # %d", ix);
        gen[ix]->begin(files[ix], stubs[ix]);
        states[ix] = PLAY;
        break;

    case PLAY:
        log_d("Pausing track # %d", ix);
        states[ix] = PAUSE;
        stubs[ix]->stop();
        break;

    case PAUSE:
        #ifdef AUDIO_ONE_TRACK_ONLY
        for (uint8_t i = 0; i < TILES_CNT; i++)
            if (states[i] == PLAY) {
                log_d("Pausing track # %d", i);
                stubs[i]->stop();
                states[i] = PAUSE;
            }
        #endif
        log_d("Resuming track # %d", ix);
        states[ix] = PLAY;
        stubs[ix]->begin();
        break;

    case STOP:
        delete files[ix];
        auto file = fs->getFileByIndex(ix);
        files[ix] = new AudioFileSourceSD(file.audio.c_str());
        delete gen[ix];
        gen[ix] = new AudioGeneratorWAV();
        gen[ix]->begin(files[ix], stubs[ix]);
        states[ix] = PLAY;
        break;
    }
}

void Player::loop()
{
    for (uint8_t ix = 0; ix < TILES_CNT; ix++)
    {
        if (states[ix] == PLAY)
        {
            if (gen[ix]->isRunning())
            {
                if (!gen[ix]->loop())
                {
                    gen[ix]->stop();
                    stubs[ix]->stop();

                    #ifdef AUDIO_LOOP
                    log_d("Loop track # %d", ix);
                    delete files[ix];
                    auto file = fs->getFileByIndex(ix);
                    files[ix] = new AudioFileSourceSD(file.audio.c_str());
                    delete gen[ix];
                    gen[ix] = new AudioGeneratorWAV();
                    gen[ix]->begin(files[ix], stubs[ix]);
                    #else
                    log_d("Stopping track # %d", ix);
                    states[ix] = STOP;
                    #endif
                }
            }
            // else
            // {
            //     log_w("Track # %d is not running", ix);
            // }
        }
    }
}

void Player::debug()
{
    for (uint8_t ix = 0; ix < TILES_CNT; ix++)
    {
        Serial.printf("%d[%c%c|%d..%d] ",
                      ix,
                      StateSymbols[(uint8_t)states[ix]],
                      gen[ix]->isRunning() ? 'r' : '.',
                      files[ix]->getPos() >> 10,
                      files[ix]->getSize() >> 10);
    }
    Serial.println();
}