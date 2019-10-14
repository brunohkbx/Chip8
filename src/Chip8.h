#pragma once

#include "Memory.h"
#include "Cpu.h"
#include "Display.h"
#include "Keypad.h"
#include "Beeper.h"

class Chip8 {
public:
    Chip8() : cpu(memory, display, keypad) {};
    void cycle();
    const void* getDisplayData() const;
    void decrementTimers();

    Memory memory;
    Keypad keypad;
    Beeper beeper;

private:
    CPU cpu;
    Display display;
};
