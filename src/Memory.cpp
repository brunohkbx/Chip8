#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include "Memory.h"

Memory::Memory() {
	std::copy(FONTSET.begin(), FONTSET.end(), page.begin());
}

void Memory::loadRom(std::string path) {
	std::ifstream rom(path, std::ios::in | std::ios::binary);
	std::vector<uint8_t> data(std::istream_iterator<uint8_t>(rom), {});

	// TODO: Check size of ROM
	std::copy(data.begin(), data.end(), page.begin() + PROGRAM_SPACE);
}
