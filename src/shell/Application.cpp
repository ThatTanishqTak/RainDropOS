#include "Application.h"
#include <iostream>

Application::Application() = default;
Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";

        return false;
    }

    m_Window = SDL_CreateWindow("RainDrop Shell", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);

    if (!m_Window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";

        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_Renderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        
        return false;
    }

    m_Running = true;
    
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        HandleEvents();
        Render();
        
        SDL_Delay(16);
    }
}

void Application::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_Running = false;
        }
    }
}

void Application::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 15, 15, 25, 255);
    SDL_RenderClear(m_Renderer);
    SDL_RenderPresent(m_Renderer);
}

void Application::Shutdown()
{
    if (m_Renderer)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }

    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }

    SDL_Quit();
}