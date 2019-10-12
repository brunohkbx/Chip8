#pragma once

#include <cstdint>
#include <tuple>
#include "Memory.h"
#include "Display.h"

class CPU {
public:
    CPU(Memory& memory, Display& display) : memory(memory), display(display) {};
    void executeInstruction();
    void incrementCounter();

private:
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn(uint16_t nnn);
    void OP_2nnn(uint16_t nnn);
    void OP_3xkk(uint8_t x, uint8_t kk);
    void OP_4xkk(uint8_t x, uint8_t kk);
    void OP_5xy0(uint8_t x, uint8_t y);
    void OP_6xkk(uint8_t x, uint8_t kk);
    void OP_7xkk(uint8_t x, uint8_t kk);
    void OP_9xy0(uint8_t x, uint8_t y);
    void OP_Annn(uint16_t nnn);
    void OP_Bnnn(uint16_t nnn);
    void OP_Dxyn(uint8_t x, uint8_t y, uint8_t rows);
    uint16_t fetchOpcode() const;
    std::tuple<uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t> decodeOpcode(uint16_t opcode) const;

    Memory& memory;
    Display& display;
    uint16_t index{};
    uint16_t PC = 0x200;
    std::array<uint8_t, 16> registers{};
    uint8_t SP{};
    std::array<uint16_t, 16> stack{};
};
