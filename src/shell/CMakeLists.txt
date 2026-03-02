find_package(SDL2 REQUIRED)

add_executable(raindrop-shell
    main.cpp
    Application.cpp
    InputManager.cpp
    ScreenStack.cpp
    LibraryScreen.cpp
    Renderer.cpp
)

target_include_directories(raindrop-shell PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(raindrop-shell PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_ttf
)