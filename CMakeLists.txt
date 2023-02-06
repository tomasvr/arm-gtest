cmake_minimum_required(VERSION 3.13)
project(blackpill C CXX ASM)

# --- HEADER AND SOURCE FILES
include_directories(inc)
file(GLOB C_SRCS "src/*.c")
file(GLOB C_SRCS_TARGET "src/target/*.c")
file(GLOB C_SRCS_MOCK "test/mocks/*.c")
file(GLOB CXX_SRCS "")
file(GLOB CXX_SRCS_TEST "test/*.cc")
file(GLOB ASM_SRCS "")
add_compile_options(-Wall -Wextra)

# Include libopencm3 configuration for stm32f1
include(cmake/libopencm3_stm32f1.cmake)

# Create target executable (hex and bin files)
include(cmake/target.cmake)

# Create unit test executable
include(cmake/gtest.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)