#pragma once

#include <AudioOutputI2S.h>
#include <AudioGeneratorWAV.h>
#include "AudioOutputMixer.h"

#include "filesystem.hpp"

#define TILES_CNT (BRD_ROWS * BRD_COLS)
#define LOOP_TRACKS false

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
        gen[ix] = new AudioGeneratorWAV();
    }
}

void Player::toggle(uint8_t ix)
{
    switch (states[ix])
    {
    case INIT:
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
                    log_d("Stopping track # %d", ix);
                    gen[ix]->stop();
                    stubs[ix]->stop();
                    states[ix] = (LOOP_TRACKS) ? INIT : STOP;
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
        Serial.printf("[%c%c|%c %d .. %d]",
                      StateSymbols[(uint8_t)states[ix]],
                      gen[ix]->isRunning() ? 'r' : '.',
                      files[ix]->isOpen() ? 'o' : 'c',
                      files[ix]->getPos() >> 10,
                      files[ix]->getSize() >> 10);
    }
    Serial.println();
}