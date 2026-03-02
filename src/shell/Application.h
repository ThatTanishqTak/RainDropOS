#pragma once

#include <SDL2/SDL.h>

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

    SDL_Window* m_Window   = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    bool m_Running  = false;
};