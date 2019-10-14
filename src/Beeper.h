#pragma once

#include "SDL.h"

class Beeper {
public:
    Beeper();
    void beep();

private:
    SDL_AudioSpec wavSpec;
    Uint8* wavBuffer;
    Uint32 wavLength;
    SDL_AudioDeviceID deviceId{};
};
