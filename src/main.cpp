#include <iostream>
#include "Chip8.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    Chip8 chip8;
    Renderer renderer("CHIP-8 Emulator");
    bool quit = false;

    chip8.memory.loadRom(argv[1]);

    while (!quit) {
        chip8.cycle();

        renderer.update(chip8.getDisplayData());

        quit = renderer.handleInput(chip8.keypad);
    }

    SDL_Quit();
    exit(0);
}
