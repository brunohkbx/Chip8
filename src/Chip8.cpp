#include "Chip8.h"
#include <iostream>
#include "Cpu.h"

void Chip8::cycle() {
    if (cpu.soundTimer == 1)
        beeper.beep();

    cpu.executeInstruction();
}

const void* Chip8::getDisplayData() const {
    return display.memory.data();
}
