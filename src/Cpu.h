#pragma once

#include <cstdint>
#include <tuple>
#include <map>
#include <functional>
#include "Memory.h"
#include "Display.h"

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/
struct Opcode {
public:
    Opcode(uint16_t opcode) :
        instruction(opcode),
        operation((opcode & 0xF000) >> 12),
        nnn(opcode & 0x0FFF),
        n(opcode & 0xF),
        x((opcode >> 8) & 0xF),
        y((opcode >> 4) & 0xF),
        kk(opcode & 0x00FF)
    {};

    const uint16_t instruction;
    const uint8_t operation;
    const uint16_t nnn;
    const uint8_t n;
    const uint8_t x;
    const uint8_t y;
    const uint8_t kk;
};

class CPU {
public:
    CPU(Memory& memory, Display& display) : memory(memory), display(display) {
        dispatchTable.emplace(0x0, [this](Opcode opcode) { executeOP_00(opcode); });
        dispatchTable.emplace(0x1, [this](Opcode opcode) { OP_1nnn(opcode); });
        dispatchTable.emplace(0x2, [this](Opcode opcode) { OP_2nnn(opcode); });
        dispatchTable.emplace(0x3, [this](Opcode opcode) { OP_3xkk(opcode); });
        dispatchTable.emplace(0x4, [this](Opcode opcode) { OP_4xkk(opcode); });
        dispatchTable.emplace(0x5, [this](Opcode opcode) { OP_5xy0(opcode); });
        dispatchTable.emplace(0x6, [this](Opcode opcode) { OP_6xkk(opcode); });
        dispatchTable.emplace(0x7, [this](Opcode opcode) { OP_7xkk(opcode); });
        dispatchTable.emplace(0x8, [this](Opcode opcode) { executeOP_08(opcode); });
        dispatchTable.emplace(0x9, [this](Opcode opcode) { OP_9xy0(opcode); });
        dispatchTable.emplace(0xA, [this](Opcode opcode) { OP_Annn(opcode); });
        dispatchTable.emplace(0xB, [this](Opcode opcode) { OP_Bnnn(opcode); });
        dispatchTable.emplace(0xD, [this](Opcode opcode) { OP_Dxyn(opcode); });
        dispatchTable.emplace(0xF, [this](Opcode opcode) { executeOP_0F(opcode); });
    }

    void executeInstruction();
    void incrementCounter();

private:
    Opcode fetchOpcode() const;
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn(Opcode opcode);
    void OP_2nnn(Opcode opcode);
    void OP_3xkk(Opcode opcode);
    void OP_4xkk(Opcode opcode);
    void OP_5xy0(Opcode opcode);
    void OP_6xkk(Opcode opcode);
    void OP_7xkk(Opcode opcode);
    void OP_8xy0(Opcode opcode);
    void OP_8xy1(Opcode opcode);
    void OP_8xy2(Opcode opcode);
    void OP_8xy3(Opcode opcode);
    void OP_8xy4(Opcode opcode);
    void OP_8xy5(Opcode opcode);
    void OP_8xy6(Opcode opcode);
    void OP_8xy7(Opcode opcode);
    void OP_8xyE(Opcode opcode);
    void OP_9xy0(Opcode opcode);
    void OP_Annn(Opcode opcode);
    void OP_Bnnn(Opcode opcode);
    void OP_Dxyn(Opcode opcode);
    void OP_Fx07(Opcode opcode);
    void OP_Fx0A(Opcode opcode);
    void OP_Fx15(Opcode opcode);
    void OP_Fx18(Opcode opcode);
    void OP_Fx1E(Opcode opcode);
    void OP_Fx29(Opcode opcode);
    void OP_Fx33(Opcode opcode);
    void OP_Fx55(Opcode opcode);
    void OP_Fx65(Opcode opcode);
    void executeOP_00(Opcode opcode);
    void executeOP_08(Opcode opcode);
    void executeOP_0F(Opcode opcode);

    Memory& memory;
    Display& display;
    uint16_t index{};
    uint16_t PC = 0x200;
    std::array<uint8_t, 16> registers{};
    uint8_t SP{};
    std::array<uint16_t, 16> stack{};
    std::map<uint8_t, std::function<void(Opcode)>> dispatchTable;
};
