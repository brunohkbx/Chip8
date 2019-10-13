#pragma once

#include <array>
#include <map>
#include "SDL.h"

class Keypad {
public:
    std::array<uint8_t, 16> memory{};
    void setKey(SDL_Keycode keyboardKey, uint8_t value);

private:
    static const std::map<int, SDL_Keycode> LAYOUT;
};
