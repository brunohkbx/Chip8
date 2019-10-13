#include "Keypad.h"

/*
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
*/
const std::map<int, SDL_Keycode> Keypad::LAYOUT = {
    { SDL_GetKeyFromName("X"), 0x0},
    { SDL_GetKeyFromName("1"), 0x1},
    { SDL_GetKeyFromName("2"), 0x2},
    { SDL_GetKeyFromName("3"), 0x3},
    { SDL_GetKeyFromName("Q"), 0x4},
    { SDL_GetKeyFromName("W"), 0x5},
    { SDL_GetKeyFromName("E"), 0x6},
    { SDL_GetKeyFromName("A"), 0x7},
    { SDL_GetKeyFromName("S"), 0x8},
    { SDL_GetKeyFromName("D"), 0x9},
    { SDL_GetKeyFromName("Z"), 0xA},
    { SDL_GetKeyFromName("C"), 0xB},
    { SDL_GetKeyFromName("4"), 0xC},
    { SDL_GetKeyFromName("R"), 0xD},
    { SDL_GetKeyFromName("F"), 0xE},
    { SDL_GetKeyFromName("V"), 0xF},
};

void Keypad::setKey(SDL_Keycode keyboardKey, uint8_t value) {
    int keypadKey = LAYOUT.at(keyboardKey);

    memory.at(keypadKey) = value;
}
