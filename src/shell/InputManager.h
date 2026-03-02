#pragma once

#include <SDL2/SDL.h>

enum class Action
{
    None,
    Up,
    Down,
    Left,
    Right,
    Confirm,
    Back,
    Menu
};

class InputManager
{
public:
    void ProcessEvent(const SDL_Event& event);
    Action ConsumeAction();

private:
    Action m_PendingAction = Action::None;
};