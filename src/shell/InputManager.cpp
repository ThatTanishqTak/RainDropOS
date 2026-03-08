#include "InputManager.h"

void InputManager::ProcessEvent(const SDL_Event& event)
{
    Action action = Action::None;

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                action = Action::Up;
                break;
            case SDLK_DOWN:
                action = Action::Down;
                break;
            case SDLK_LEFT:
                action = Action::Left;
                break;
            case SDLK_RIGHT:
                action = Action::Right;
                break;
            case SDLK_RETURN:
                action = Action::Confirm;
                break;
            case SDLK_ESCAPE:
                action = Action::Back;
                break;
            case SDLK_TAB:
                action = Action::Menu;
                break;
            default:
                break;
        }
    }

    if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        switch (event.cbutton.button)
        {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                action = Action::Up;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                action = Action::Down;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                action = Action::Left;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                action = Action::Right;
                break;
            case SDL_CONTROLLER_BUTTON_A:
                action = Action::Confirm;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                action = Action::Back;
                break;
            case SDL_CONTROLLER_BUTTON_START:
                action = Action::Menu;
                break;
            default:
                break;
        }
    }

    if (action != Action::None)
    {
        m_ActionQueue.push(action);
    }
}

bool InputManager::HasActions() const
{
    return !m_ActionQueue.empty();
}

Action InputManager::ConsumeAction()
{
    if (m_ActionQueue.empty())
    {
        return Action::None;
    }

    Action action = m_ActionQueue.front();
    m_ActionQueue.pop();
    
    return action;
}