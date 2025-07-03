cmake_minimum_required(VERSION 3.20)
project(RainDropOS LANGUAGES C CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(RainDropOS STATIC
    src/Main.cpp
)

target_include_directories(RainDropOS PUBLIC include)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

option(BUILD_TESTING "Enable testing" ON)
if(BUILD_TESTING)
    enable_testing()
    add_executable(RainDropOS_tests
        test/TestMain.cpp
    )
    target_include_directories(RainDropOS_tests PRIVATE include)
    target_link_libraries(RainDropOS_tests PRIVATE Catch2::Catch2WithMain)
    add_test(NAME RainDropOSTests COMMAND RainDropOS_tests)
endif()