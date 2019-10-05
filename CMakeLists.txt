cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Chip8)

# Find all executables
file(GLOB PROJECT_SOURCE_DIR *.cpp)

# Add project executable
add_executable(Chip8 ${PROJECT_SOURCE_DIR})
