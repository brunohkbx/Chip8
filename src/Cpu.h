#pragma once

#include <array>
#include <vector>
#include "Memory.h"

class Cpu {
private:
	Memory memory;
	std::array<uint8_t, 16> V = { 0 };
	uint16_t PC = 0x200;
	uint8_t SP;
	std::vector<uint16_t> stack;
};
