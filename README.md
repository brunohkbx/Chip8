# Chip8

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Chip8 <rom_name>`. You can choose one from the 23 public domain ROMs in the `roms` directory.

## Testing the implementation
There are two roms that can be used to check the CPU:

##### [BC_test](https://slack-files.com/T3CH37TNX-F3RKEUKL4-b05ab4930d)
`./Chip8 ../roms/test/BC_test.ch8`

##### [chip8-test-rom](https://github.com/corax89/chip8-test-rom)
`./Chip8 ../roms/test/chip8-test-rom.ch8`
