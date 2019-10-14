#include "Renderer.h"
#include "Display.h"

Renderer::Renderer(std::string title) {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        SDL_Log("%s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
        SDL_Log("%s", SDL_GetError());
        return;
    }

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, Display::WIDTH, Display::HEIGHT
    );

    if (texture == nullptr) {
        SDL_Log("%s", SDL_GetError());
        return;
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

void Renderer::update(const void* pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, Display::WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Renderer::handleInput(Keypad& keypad) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            keypad.setKey(event.key.keysym.sym, 1);
            break;
        case SDL_KEYUP:
            keypad.setKey(event.key.keysym.sym, 0);
            break;
        default:
            return false;
        }
    }
}
