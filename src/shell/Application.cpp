#include "Application.h"
#include "LibraryScreen.h"

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

    m_SDLRenderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_SDLRenderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        return false;
    }

    if (!m_Renderer.Initialize(m_SDLRenderer))
    {
        return false;
    }

    m_Screens.Push(std::make_unique<LibraryScreen>());

    m_Running = true;
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        HandleEvents();

        if (m_Screens.IsEmpty())
        {
            m_Running = false;
        }

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

        m_Input.ProcessEvent(event);
    }

    Action action = m_Input.ConsumeAction();
    m_Screens.Update(action);
}

void Application::Render()
{
    m_Screens.Render(m_Renderer);
}

void Application::Shutdown()
{
    m_Renderer.Shutdown();
    
    if (m_SDLRenderer)
    {
        SDL_DestroyRenderer(m_SDLRenderer);
        m_SDLRenderer = nullptr;
    }
    
    if (m_Window)
    {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    
    SDL_Quit();
}