#include <iostream>
#include "Chip8.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    Chip8 chip8;
    Renderer renderer("CHIP-8 Emulator");
    SDL_Event e;

    chip8.memory.loadRom(argv[1]);

    while (true) {
        chip8.cycle();

        renderer.update(chip8.getDisplayData());

        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    exit(0);
            }
        }
    }
}
