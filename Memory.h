#pragma once

#include <array>
#include <string>

/*
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
*/
class Memory {
public:
	void loadRom(std::string path);

public:
	static constexpr int MAX_MEMORY = 4096;
	static constexpr int PROGRAM_SPACE = 0x200;
	static constexpr int MAX_PROGRAM_SIZE = MAX_MEMORY - PROGRAM_SPACE;

	std::array<uint8_t, MAX_MEMORY> page = { 0 };
};