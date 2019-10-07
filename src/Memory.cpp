#include "Memory.h"
#include <fstream>
#include <iterator>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Memory::Memory() {
    std::copy(FONTSET.begin(), FONTSET.end(), page.begin());
}

void Memory::loadRom(std::string path) {
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data(std::istream_iterator<uint8_t>(rom), {});

    try {
        auto size = fs::file_size(path);

        if (size > MAX_PROGRAM_SIZE) {
            std::cout << "Program exceeds maximum size of " << MAX_PROGRAM_SIZE << std::endl;
        }
        else {
            std::copy(data.begin(), data.end(), page.begin() + PROGRAM_SPACE);
        }
    }
    catch (fs::filesystem_error & e) {
        std::cout << e.what() << '\n';
    }
}
