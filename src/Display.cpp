#include "Display.h"

uint32_t Display::getPixel(uint8_t y, int row, uint8_t x, int column) const {
    int index = (y + row) * WIDTH + (x + column);

    return memory.at(index);
}

void Display::setPixel(uint8_t y, int row, uint8_t x, int column) {
    int index = (y + row) * WIDTH + (x + column);

    memory.at(index) = memory.at(index) ^ 0xFFFFFFFF;
}
