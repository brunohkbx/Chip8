#pragma once

#include <array>

class Display {
public:
    uint32_t getPixel(uint8_t y, int row, uint8_t x, int column) const;
    void setPixel(uint8_t y, int row, uint8_t x, int column);

    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 32;
    std::array<uint32_t, WIDTH * HEIGHT> memory{};
};
