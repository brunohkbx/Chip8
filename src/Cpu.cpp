#include "Cpu.h"
#include <iostream>

void CPU::executeInstruction() {
    Opcode opcode = fetchOpcode();

    incrementCounter();

    try {
        dispatchTable.at(opcode.operation)(opcode);
    }
    catch (const std::out_of_range & e) {
        std::cout << "Invalid opcode.";
    }
}

void CPU::incrementCounter() {
    PC += 2;
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
Opcode CPU::fetchOpcode() const {
    auto page = memory.getPage();

    return Opcode(page.at(PC) << 8 | page.at(PC + 1));
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
void CPU::OP_1nnn(Opcode opcode) {
    PC = opcode.nnn;
}

/*
2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
*/
void CPU::OP_2nnn(Opcode opcode) {
    stack.at(SP) = PC;
    SP++;
    PC = opcode.nnn;
}

/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
*/
void CPU::OP_3xkk(Opcode opcode) {
    if (registers.at(opcode.x) == opcode.kk)
        PC += 2;
}

/*
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
*/
void CPU::OP_4xkk(Opcode opcode) {
    if (registers.at(opcode.x) != opcode.kk)
        PC += 2;
}

/*
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
*/
void CPU::OP_5xy0(Opcode opcode) {
    if (registers.at(opcode.x) == registers.at(opcode.y)) {
        PC += 2;
    }
}

/*
6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.
*/
void CPU::OP_6xkk(Opcode opcode) {
    registers.at(opcode.x) = opcode.kk;
}

/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.
*/
void CPU::OP_7xkk(Opcode opcode) {
    registers.at(opcode.x) = registers.at(opcode.x) + opcode.kk;
}

/*
9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
*/
void CPU::OP_9xy0(Opcode opcode) {
    if (registers.at(opcode.x) != registers.at(opcode.y)) {
        PC += 2;
    }
}

/*
Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.
*/
void CPU::OP_Annn(Opcode opcode) {
    index = opcode.nnn;
}

/*
Bnnn - JP V0, addr
Jump to location nnn + V0.

The program counter is set to nnn plus the value of V0.
*/
void CPU::OP_Bnnn(Opcode opcode) {
    PC = registers.at(0) + opcode.nnn;
}

/*
DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

Iterates row by row and column by column. Its eight columns because a sprite is guaranteed to be eight pixels wide.
If a sprite pixel is on then there may be a collision with what's already being displayed, so we check if our screen pixel in the same location is set. If so we must set the VF register to express collision.
*/
void CPU::OP_Dxyn(Opcode opcode) {
    uint8_t xCord = registers.at(opcode.x) % Display::WIDTH;
    uint8_t yCord = registers.at(opcode.y) % Display::HEIGHT;

    registers.at(0xF) = 0;

    for (int row = 0; row < opcode.n; row++) {
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

void CPU::executeOP_00(Opcode opcode) {
    switch (opcode.instruction) {
    case 0x00E0:
        OP_00E0();
        break;
    case 0x00EE:
        OP_00EE();
        break;
    }
}
