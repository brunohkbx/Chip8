#include "Cpu.h"
#include <iostream>

void CPU::executeInstruction(uint16_t opcode) const {
    auto [operation, nnn, n, x, y, kk] = decodeOpcode(opcode);

    switch (operation)
    {
    case 0xD:
        draw(x, y, n);
        break;
    }
}

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/
std::tuple<int8_t, uint16_t, int8_t, int8_t, int8_t, int8_t> CPU::decodeOpcode(uint16_t opcode) const {
    uint8_t operation = (opcode & 0xF000u) >> 12u;
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t n = opcode & 0xF;
    uint8_t x = (opcode >> 8) & 0xF;
    uint8_t y = (opcode >> 4) & 0xF;
    uint8_t kk = opcode & 0x00FF;

    return { operation, nnn, n, x, y, kk };
}

/*
DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

Iterates row by row and column by column. Its eight columns because a sprite is guaranteed to be eight pixels wide.
If a sprite pixel is on then there may be a collision with what’s already being displayed, so we check if our screen pixel in the same location is set. If so we must set the VF register to express collision.
*/
void CPU::draw(int8_t Vx, int8_t Vy, int8_t rows) {
    uint8_t x = registers[Vx] % Display::WIDTH;
    uint8_t y = registers[Vy] % Display::HEIGHT;

    for(int row = 0; row < rows; row++) {
        uint8_t pixelByte = memory.getPage().at(index + row);

        for (int column = 0; column < 8; column++) {
            // The instruction (0x80(128) >> column) has a special meaning. Its binary representation is 0b10000000 so we can shift the bit by column to check if a specific bit is set.
            uint8_t pixel = pixelByte & (0x80 >> column);
            uint32_t screenPixel = display.getPixel(y, row, x, column);

            if (pixel) {
                if (screenPixel == 1)
                    registers[0xF] = 1;

                screenPixel ^= 1;
            }
        }
    }
}
