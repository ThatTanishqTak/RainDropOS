#include "Application.h"
#include "LibraryScreen.h"

#include <csignal>
#include <iostream>

Application::Application() = default;
Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    m_Settings = Settings::Load();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        
        return false;
    }

    m_Window = SDL_CreateWindow("RainDropOS Shell", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Settings.resWidth, m_Settings.resHeight, SDL_WINDOW_SHOWN);

    if (!m_Window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        
        return false;
    }

    Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
    if (m_Settings.vsync)
    {
        renderFlags |= SDL_RENDERER_PRESENTVSYNC;
    }

    m_SDLRenderer = SDL_CreateRenderer(m_Window, -1, renderFlags);
    if (!m_SDLRenderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";

        return false;
    }

    if (!m_Renderer.Initialize(m_SDLRenderer))
    {
        return false;
    }

    // Apply fullscreen from saved settings.
    ApplySettings();

    // Set up child reaping once at startup.
    struct sigaction sa{};
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_NOCLDWAIT;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, nullptr);

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        OpenController(i);
    }

    // Pass Settings& so LibraryScreen can hand it to SettingsScreen.
    m_Screens.Push(std::make_unique<LibraryScreen>(m_Settings));

    m_Running = true;

    return true;
}

void Application::Run()
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    const Uint64 targetFrameTime = freq / 60; // ~16.67ms at 60fps

    while (m_Running)
    {
        Uint64 frameStart = SDL_GetPerformanceCounter();

        HandleEvents();

        if (m_Screens.IsEmpty() || m_Screens.WantsToQuit())
        {
            m_Running = false;
        }

        if (m_Settings.dirty)
        {
            ApplySettings();
            m_Settings.dirty = false;
        }

        Render();

        Uint64 elapsed = SDL_GetPerformanceCounter() - frameStart;
        if (elapsed < targetFrameTime)
        {
            Uint64 remaining = targetFrameTime - elapsed;
            Uint64 sleepMs   = (remaining * 1000 / freq);

            if (sleepMs > 2)
            {
                SDL_Delay(static_cast<Uint32>(sleepMs - 2));
            }

            // Spin-wait the final ~2ms for precision.
            while (SDL_GetPerformanceCounter() - frameStart < targetFrameTime) {}
        }
    }
}

void Application::ApplySettings()
{
    if (!m_Window || !m_SDLRenderer)
    {
        return;
    }

    SDL_SetWindowFullscreen(m_Window, m_Settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    if (!m_Settings.fullscreen)
    {
        SDL_SetWindowSize(m_Window, m_Settings.resWidth, m_Settings.resHeight);
    }

    std::cout << "Settings applied (fullscreen=" << m_Settings.fullscreen << " res=" << m_Settings.resWidth << "x" << m_Settings.resHeight << ")\n";
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

        // Handle controller hotplug so a controller connected after
        // startup is opened, and one that is disconnected is cleaned up.
        if (event.type == SDL_CONTROLLERDEVICEADDED)
        {
            OpenController(event.cdevice.which);
        }
        else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
        {
            CloseController(event.cdevice.which);
        }

        m_Input.ProcessEvent(event);
    }

    while (m_Input.HasActions())
    {
        Action action = m_Input.ConsumeAction();
        m_Screens.Update(action);
    }
}

void Application::Render()
{
    m_Screens.Render(m_Renderer);
}

void Application::OpenController(int deviceIndex)
{
    if (!SDL_IsGameController(deviceIndex))
    {
        return;
    }

    SDL_GameController* controller = SDL_GameControllerOpen(deviceIndex);
    if (controller)
    {
        m_Controllers.push_back(controller);
        std::cout << "Controller connected: " << SDL_GameControllerName(controller) << "\n";
    }
    else
    {
        std::cerr << "Failed to open controller " << deviceIndex << ": " << SDL_GetError() << "\n";
    }
}

void Application::CloseController(SDL_JoystickID instanceId)
{
    for (auto it = m_Controllers.begin(); it != m_Controllers.end(); ++it)
    {
        SDL_Joystick* joystick = SDL_GameControllerGetJoystick(*it);
        if (SDL_JoystickInstanceID(joystick) == instanceId)
        {
            std::cout << "Controller disconnected: " << SDL_GameControllerName(*it) << "\n";
            SDL_GameControllerClose(*it);
            m_Controllers.erase(it);

            return;
        }
    }
}

void Application::Shutdown()
{
    for (SDL_GameController* controller : m_Controllers)
    {
        SDL_GameControllerClose(controller);
    }
    m_Controllers.clear();

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