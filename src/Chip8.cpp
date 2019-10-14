#include "Chip8.h"
#include <iostream>
#include "Cpu.h"

void Chip8::cycle() {
    cpu.executeInstruction();
}

const void* Chip8::getDisplayData() const {
    return display.memory.data();
}

void Chip8::decrementTimers() {
    if (cpu.delayTimer > 0)
        cpu.delayTimer--;
    if (cpu.soundTimer > 0) {
        if (cpu.soundTimer == 1)
            beeper.beep();
        cpu.soundTimer--;
    }
}
