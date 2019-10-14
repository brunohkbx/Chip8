#include <iostream>
#include <fstream>
#include "Chip8.h"
#include "Renderer.h"

constexpr int FPS = 60;
constexpr int TICKS_PER_FRAME = 1000 / FPS;
constexpr int OPERATIONS_PER_SECOND = 500;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: Chip8 <ROM file>" << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    Chip8 chip8;
    Renderer renderer("CHIP-8 Emulator");
    bool quit = false;

    try {
        chip8.memory.loadRom(argv[1]);
    }
    catch (std::ifstream::failure e) {
        std::cout << "No such file or directory. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    auto lastUpdate = SDL_GetTicks();
    while (!quit) {
        auto frameStart = SDL_GetTicks();

        quit = renderer.handleInput(chip8.keypad);

        // Delay and sound timer decrements at a rate of 60Hz
        if (SDL_GetTicks() - lastUpdate > TICKS_PER_FRAME) {
            chip8.decrementTimers();
            lastUpdate = SDL_GetTicks();
        }

        chip8.cycle();
        renderer.update(chip8.getDisplayData());

        auto frameDuration = SDL_GetTicks() - frameStart;
        if (frameDuration < 1000 / OPERATIONS_PER_SECOND)
            SDL_Delay((1000 / OPERATIONS_PER_SECOND) - frameDuration); //run at 500Hz
    }

    SDL_Quit();

    return 0;
}
