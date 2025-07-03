cmake_minimum_required(VERSION 3.20)
project(RainDropOS LANGUAGES C CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RainDropOS
    src/Main.cpp
    asm/bootstrap.asm
)

target_include_directories(RainDropOS PUBLIC include)