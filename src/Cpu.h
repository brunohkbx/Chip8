#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include "Memory.h"
#include "Display.h"

class CPU {
public:
    CPU(Memory& memory, Display& display) : memory(memory), display(display) {};
    void executeInstruction(uint16_t opcode);

private:
    void draw(uint8_t Vx, uint8_t Vy, uint8_t rows);
    std::tuple<uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t> decodeOpcode(uint16_t opcode) const;

    Memory& memory;
    Display& display;
    uint16_t index = 0;
    std::array<uint8_t, 16> registers{};
};
