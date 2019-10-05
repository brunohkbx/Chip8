#include "Display.h"

Display::Display(std::string title) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title.c_str(), 0, 0, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Display::update() {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
