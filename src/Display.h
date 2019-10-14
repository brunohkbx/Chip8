#pragma once

#include <array>

class Display {
public:
    uint32_t getPixel(uint8_t y, uint8_t x) const;
    void setPixel(uint8_t y, uint8_t x);

    static constexpr int WIDTH = 64;
    static constexpr int HEIGHT = 32;
    std::array<uint32_t, WIDTH * HEIGHT> memory{};
};
