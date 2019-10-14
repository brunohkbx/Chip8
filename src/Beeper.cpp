#include "Beeper.h"
#include <iostream>

Beeper::Beeper() {
    if (SDL_LoadWAV("src/resources/buzz.wav", &wavSpec, &wavBuffer, &wavLength) == nullptr)
        SDL_Log("%s", SDL_GetError());

    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0)
        SDL_Log("Failed to open audio: %s", SDL_GetError());
}

Beeper::~Beeper() {
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Beeper::beep() {
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}
