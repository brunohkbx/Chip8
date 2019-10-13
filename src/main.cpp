#include <iostream>
#include <fstream>
#include "Chip8.h"
#include "Renderer.h"

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
        if (errno)
            std::cout << strerror(errno) << std::endl;
        else
            std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    while (!quit) {
        chip8.cycle();

        renderer.update(chip8.getDisplayData());

        quit = renderer.handleInput(chip8.keypad);
    }

    SDL_Quit();

    return 0;
}
