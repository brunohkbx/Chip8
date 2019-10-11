#include "Memory.h"
#include <fstream>
#include <iterator>
#include <iostream>

Memory::Memory() {
    std::copy(FONTSET.begin(), FONTSET.end(), page.begin());
}

void Memory::loadRom(std::string path) {
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(rom)), std::istreambuf_iterator<char>());

    if (data.size() > MAX_PROGRAM_SIZE) {
        std::cout << "Program exceeds maximum size of " << MAX_PROGRAM_SIZE << std::endl;
    }
    else {
        std::copy(data.begin(), data.end(), page.begin() + PROGRAM_SPACE);
    }
}
