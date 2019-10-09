#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include "Memory.h"
#include "Display.h"

class CPU {
public:
    CPU(Memory& memory, Display& display) : memory(memory), display(display) {};
    void executeInstruction(uint16_t opcode) const;

private:
    void draw(int8_t Vx, int8_t Vy, int8_t rows);
    std::tuple<int8_t, uint16_t, int8_t, int8_t, int8_t, int8_t> decodeOpcode(uint16_t opcode) const;

    Memory& memory;
    Display& display;
    uint16_t index = 0;
    std::array<uint8_t, 16> registers{};
};
