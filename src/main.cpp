#include <iostream>
#include "Display.h"
#include "Chip8.h"

int main(int argc, char* argv[])
{   
    Chip8 chip8;
    Display display("CHIP-8 Emulator");
    SDL_Event e;

    while (true) {
        display.update();

        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    exit(0);
            }
        }
    }
}
