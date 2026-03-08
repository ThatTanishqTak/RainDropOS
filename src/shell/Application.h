#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "InputManager.h"
#include "ScreenStack.h"
#include "Renderer.h"
#include "Settings.h"

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
    void ApplySettings();
    void OpenController(int deviceIndex);
    void CloseController(SDL_JoystickID instanceId);

    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_SDLRenderer = nullptr;
    bool m_Running = false;

    Settings m_Settings;
    InputManager m_Input;
    ScreenStack m_Screens;
    Renderer m_Renderer;

    std::vector<SDL_GameController*> m_Controllers;
};