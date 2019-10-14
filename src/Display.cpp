#include "Display.h"
#include <iostream>

uint32_t Display::getPixel(uint8_t y, uint8_t x) const {
    int index = y * WIDTH + x;

    return memory.at(index);
}

void Display::setPixel(uint8_t y, uint8_t x) {
    int index = y * WIDTH + x;

    memory.at(index) = memory.at(index) ^ 0xFFFFFFFF;
}
