#include <iostream>
#include "Display.h"

int main()
{
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
