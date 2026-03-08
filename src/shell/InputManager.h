#pragma once

#include <SDL2/SDL.h>
#include <queue>

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
    bool HasActions() const;
    Action ConsumeAction();

private:
    std::queue<Action> m_ActionQueue;
};