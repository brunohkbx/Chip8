#pragma once

#include <array>
#include <vector>
#include "Memory.h"

/*
Chip-8 has 16 general-purpose 8-bit registers, usually referred to as Vx, where x is a hexadecimal digit (0 through F). There is also a 16-bit register called I. 
Most Chip-8 programs start at location 0x200 (512)
The program counter (PC) should be 16-bit and is used to store the currently executing address. The stack pointer (SP) can be 8-bit, it is used to point to the topmost level of the stack.
The stack is an array of 16 16-bit values, used to store the address that the interpreter should  return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
The original implementation of the Chip-8 language used a 64x32-pixel monochrome display
*/

class Chip8 {
public:
    static constexpr int DISPLAY_SIZE = 64 * 32;
public:
    std::array<uint32_t, DISPLAY_SIZE> video{};

private:
    Memory memory;
    std::array<uint8_t, 16> V{};
    uint16_t PC = 0x200;
    uint8_t SP;
    std::vector<uint16_t> stack;
};
