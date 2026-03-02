#include "InputManager.h"

void InputManager::ProcessEvent(const SDL_Event& event)
{
    // Keyboard input (for development)
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                m_PendingAction = Action::Up;
                break;
            case SDLK_DOWN:
                m_PendingAction = Action::Down;
                break;
            case SDLK_LEFT:
                m_PendingAction = Action::Left;
                break;
            case SDLK_RIGHT: 
                m_PendingAction = Action::Right;  
                break;
            case SDLK_RETURN:
                m_PendingAction = Action::Confirm;
                break;
            case SDLK_ESCAPE:
                m_PendingAction = Action::Back;
                break;
            case SDLK_TAB:
                m_PendingAction = Action::Menu;
                break;
            default:
                break;
        }
    }

    // Controller input
    if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        switch (event.cbutton.button)
        {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                m_PendingAction = Action::Up;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                m_PendingAction = Action::Down;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                m_PendingAction = Action::Left;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                m_PendingAction = Action::Right;
                break;
            case SDL_CONTROLLER_BUTTON_A:
                m_PendingAction = Action::Confirm;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                m_PendingAction = Action::Back;
                break;
            case SDL_CONTROLLER_BUTTON_START:
                m_PendingAction = Action::Menu;
                break;
            default:
                break;
        }
    }
}

Action InputManager::ConsumeAction()
{
    Action action = m_PendingAction;
    m_PendingAction = Action::None;

    return action;
}