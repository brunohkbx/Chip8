#pragma once

#include "Memory.h"
#include "Cpu.h"
#include "Display.h"
#include "Keypad.h"

class Chip8 {
public:
    Chip8() : cpu(memory, display, keypad) {};
    void cycle();
    const void* getDisplayData() const;

    Memory memory;
    Keypad keypad;

private:
    CPU cpu;
    Display display;
};
