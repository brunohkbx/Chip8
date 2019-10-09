#include "Chip8.h"
#include <iostream>
#include "Cpu.h"

void Chip8::cycle() {
    uint16_t opcode = fetchOpcode();

    cpu.executeInstruction(opcode);

    PC += 2;
}

void* Chip8::getDisplayData() const {
    return display.memory.data();
}

/*
All instructions are 2 bytes long, this method fetches two successive bytes and merges them to get the actual opcode.
0xA2       0xA2 << 8 = 0xA200   HEX
10100010   1010001000000000     BIN

1010001000000000 | // 0xA200
        11110000 = // 0xF0 (0x00F0)
------------------
1010001011110000   // 0xA2F0
*/

uint16_t Chip8::fetchOpcode() const {
    auto page = memory.getPage();

    return page.at(PC) << 8 | page.at(PC + 1);
}
