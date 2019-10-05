#pragma once

#include <string>
#include "SDL.h"

class Display {
public:
    Display(std::string title);
    void update();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
