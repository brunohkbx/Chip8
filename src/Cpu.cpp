#include "Cpu.h"
#include <iostream>

void CPU::executeInstruction() {
    uint16_t opcode = fetchOpcode();
    auto [operation, nnn, n, x, y, kk] = decodeOpcode(opcode);

    incrementCounter();

    switch (operation)
    {
    case 0x0:
        switch (opcode) {
        case 0x00E0:
            OP_00E0();
            break;
        case 0x00EE:
            OP_00EE();
            break;
        }
        break;
    case 0x1:
        OP_1nnn(nnn);
        break;
    case 0x02:
        OP_2nnn(nnn);
        break;
    case 0x3:
        OP_3xkk(x, kk);
        break;
    case 0x4:
        OP_4xkk(x, kk);
        break;
    case 0x5:
        OP_5xy0(x, y);
        break;
    case 0x6:
        OP_6xkk(x, kk);
        break;
    case 0x7:
        OP_7xkk(x, kk);
        break;
    case 0x9:
        OP_9xy0(x, y);
        break;
    case 0xA:
        OP_Annn(nnn);
        break;
    case 0xB:
        OP_Bnnn(nnn);
        break;
    case 0xD:
        OP_Dxyn(x, y, n);
        break;
    }
}

void CPU::incrementCounter() {
    PC += 2;
}

/*
00E0 - CLS
Clear the display.
*/
void CPU::OP_00E0() {
    display.memory.fill(0);
}

/*
00EE - RET
Return from a subroutine.

The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
*/
void CPU::OP_00EE() {
    SP--;
    PC = stack.at(SP);
}

/*
1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.
*/
void CPU::OP_1nnn(uint16_t nnn) {
    PC = nnn;
}

/*
2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
*/
void CPU::OP_2nnn(uint16_t nnn) {
    stack.at(SP) = PC;
    SP++;
    PC = nnn;
}

/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
*/
void CPU::OP_3xkk(uint8_t x, uint8_t kk) {
    if (registers.at(x) == kk)
        PC += 2;
}

/*
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
*/
void CPU::OP_4xkk(uint8_t x, uint8_t kk) {
    if (registers.at(x) != kk)
        PC += 2;
}

/*
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
*/
void CPU::OP_5xy0(uint8_t x, uint8_t y) {
    if (registers.at(x) == registers.at(y)) {
        PC += 2;
    }
}

/*
6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.
*/
void CPU::OP_6xkk(uint8_t x, uint8_t kk) {
    registers.at(x) = kk;
}

/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.
*/
void CPU::OP_7xkk(uint8_t x, uint8_t kk) {
    registers.at(x) = registers.at(x) + kk;
}

/*
9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
*/
void CPU::OP_9xy0(uint8_t x, uint8_t y) {
    if (registers.at(x) != registers.at(y)) {
        PC += 2;
    }
}

/*
Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.
*/
void CPU::OP_Annn(uint16_t nnn) {
    index = nnn;
}

/*
Bnnn - JP V0, addr
Jump to location nnn + V0.

The program counter is set to nnn plus the value of V0.
*/
void CPU::OP_Bnnn(uint16_t nnn) {
    PC = registers.at(0) + nnn;
}

/*
DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

Iterates row by row and column by column. Its eight columns because a sprite is guaranteed to be eight pixels wide.
If a sprite pixel is on then there may be a collision with what's already being displayed, so we check if our screen pixel in the same location is set. If so we must set the VF register to express collision.
*/
void CPU::OP_Dxyn(uint8_t x, uint8_t y, uint8_t rows) {
    uint8_t xCord = registers.at(x) % Display::WIDTH;
    uint8_t yCord = registers.at(y) % Display::HEIGHT;

    registers.at(0xF) = 0;

    for (int row = 0; row < rows; row++) {
        uint8_t pixelByte = memory.getPage().at(index + row);

        for (int column = 0; column < 8; column++) {
            // The instruction (0x80(128) >> column) has a special meaning. Its binary representation is 0b10000000 so we can shift the bit by column to check if a specific bit is set.
            uint8_t pixel = pixelByte & (0x80 >> column);
            uint32_t screenPixel = display.getPixel(yCord, row, xCord, column);

            if (pixel) {
                if (screenPixel == 0xFFFFFFFF)
                    registers.at(0xF) = 1;

                display.setPixel(yCord, row, xCord, column);
            }
        }
    }
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
uint16_t CPU::fetchOpcode() const {
    auto page = memory.getPage();

    return page.at(PC) << 8 | page.at(PC + 1);
}

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/
std::tuple<uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t> CPU::decodeOpcode(uint16_t opcode) const {
    uint8_t operation = (opcode & 0xF000) >> 12;
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t n = opcode & 0xF;
    uint8_t x = (opcode >> 8) & 0xF;
    uint8_t y = (opcode >> 4) & 0xF;
    uint8_t kk = opcode & 0x00FF;

    return { operation, nnn, n, x, y, kk };
}
