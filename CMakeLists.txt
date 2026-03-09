cmake_minimum_required(VERSION 3.20)
project(RainDropOS VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_subdirectory(src/shell)

add_executable(vdf-test
    tests/vdf_test.cpp
    src/shell/VdfParser.cpp
)
target_include_directories(vdf-test PRIVATE src/shell)

add_executable(steam-test
    tests/steam_test.cpp
    src/shell/SteamScanner.cpp
    src/shell/VdfParser.cpp
)
target_include_directories(steam-test PRIVATE src/shell)

add_executable(ipc-test
    tests/ipc_test.cpp
)
target_include_directories(ipc-test PRIVATE include)

add_executable(ipc-comms-test
    tests/ipc_comms_test.cpp
    src/daemons/IpcServer.cpp
    src/shell/IpcClient.cpp
)
target_include_directories(ipc-comms-test PRIVATE
    src/daemons
    src/shell
    include
)