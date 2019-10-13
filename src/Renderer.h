#pragma once

#include <string>
#include "SDL.h"
#include "Keypad.h"

class Renderer {
public:
    Renderer(std::string title);
    ~Renderer();
    void update(const void* pixels);
    bool handleInput(Keypad& keypad);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
