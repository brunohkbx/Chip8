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
8xy0 - LD Vx, Vy
Set Vx = Vy.

Stores the value of register Vy in register Vx.
*/
void CPU::OP_8xy0(Opcode opcode) {
    registers.at(opcode.x) = registers.at(opcode.y);
}

/*
8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.

Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
*/
void CPU::OP_8xy1(Opcode opcode) {
    registers.at(opcode.x) = registers.at(opcode.x) | registers.at(opcode.y);
}

/*
8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.

Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
*/
void CPU::OP_8xy2(Opcode opcode) {
    registers.at(opcode.x) = registers.at(opcode.x) & registers.at(opcode.y);
}

/*
8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.

Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
*/
void CPU::OP_8xy3(Opcode opcode) {
    registers.at(opcode.x) = registers.at(opcode.x) ^ registers.at(opcode.y);
}

/*
8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.

The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
*/
void CPU::OP_8xy4(Opcode opcode) {
    int sum = registers.at(opcode.x) + registers.at(opcode.y);

    if (sum > 255)
        registers.at(0xF) = 1;
    else
        registers.at(0xF) = 0;

    registers.at(opcode.x) = sum;
}

/*
8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.

If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
*/
void CPU::OP_8xy5(Opcode opcode) {
    if (registers.at(opcode.x) > registers.at(opcode.y))
        registers.at(0xF) = 1;
    else
        registers.at(0xF) = 0;

    registers.at(opcode.x) = registers.at(opcode.x) - registers.at(opcode.y);
}

/*
8xy6 - SHR Vx {, Vy}
Set Vx = Vx SHR 1.

If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
*/
void CPU::OP_8xy6(Opcode opcode) {
    registers.at(0xF) = registers.at(opcode.x) & 0x1;

    registers.at(opcode.x) = registers.at(opcode.x) >> 1;
}

/*
8xy7 - SUBN Vx, Vy
Set Vx = Vy - Vx, set VF = NOT borrow.

If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
*/
void CPU::OP_8xy7(Opcode opcode) {
    if (registers.at(opcode.y) > registers.at(opcode.x))
        registers.at(0xF) = 1;
    else
        registers.at(0xF) = 0;

    registers.at(opcode.x) = registers.at(opcode.y) - registers.at(opcode.x);
}

/*
8xyE - SHL Vx {, Vy}
Set Vx = Vx SHL 1.

If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
*/
void CPU::OP_8xyE(Opcode opcode) {
    registers.at(0xF) = (registers.at(opcode.x) & 0x80) >> 7;

    registers.at(opcode.x) = registers.at(opcode.x) << 1;
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

void CPU::executeOP_08(Opcode opcode) {
    switch (opcode.n) {
    case 0x0:
        OP_8xy0(opcode);
        break;
    case 0x1:
        OP_8xy1(opcode);
        break;
    case 0x2:
        OP_8xy2(opcode);
        break;
    case 0x3:
        OP_8xy3(opcode);
        break;
    case 0x4:
        OP_8xy4(opcode);
        break;
    case 0x5:
        OP_8xy5(opcode);
        break;
    case 0x6:
        OP_8xy6(opcode);
        break;
    case 0x7:
        OP_8xy7(opcode);
        break;
    case 0xE:
        OP_8xyE(opcode);
        break;
    }
}
