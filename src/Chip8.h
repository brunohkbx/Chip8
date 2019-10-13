#pragma once

#include "Memory.h"
#include "Cpu.h"
#include "Display.h"

class Chip8 {
public:
    Chip8() : cpu(memory, display, keypad) {};
    void cycle();
    const void* getDisplayData() const;

    Memory memory;
    std::array<uint8_t, 16> keypad{};

private:
    CPU cpu;
    Display display;
};
