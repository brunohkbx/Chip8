#pragma once

#include <string>
#include "SDL.h"

class Renderer {
public:
    Renderer(std::string title);
    ~Renderer();
    void update(const void* pixels);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
