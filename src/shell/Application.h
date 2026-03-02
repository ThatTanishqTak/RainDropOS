#pragma once

#include <SDL2/SDL.h>

#include "InputManager.h"
#include "ScreenStack.h"
#include "Renderer.h"

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void HandleEvents();
    void Render();

    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_SDLRenderer = nullptr;
    bool m_Running = false;

    InputManager m_Input;
    ScreenStack m_Screens;
    Renderer m_Renderer;
};