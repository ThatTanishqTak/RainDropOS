#include "ScreenStack.h"

void ScreenStack::Push(std::unique_ptr<Screen> screen)
{
    screen->OnEnter();
    m_Screens.push_back(std::move(screen));
}

void ScreenStack::Pop()
{
    if (!m_Screens.empty())
    {
        m_Screens.back()->OnExit();
        m_Screens.pop_back();

        if (!m_Screens.empty())
        {
            m_Screens.back()->OnEnter();
        }
    }
}

void ScreenStack::Update(Action action)
{
    if (m_Screens.empty())
    {
        return;
    }

    auto& top = m_Screens.back();
    top->Update(action);

    if (top->HasPendingPush())
    {
        Push(top->TakePendingPush());

        return;
    }

    if (top->WantsToExit())
    {
        Pop();
    }
}

void ScreenStack::Render(Renderer& renderer)
{
    if (m_Screens.empty())
    {
        return;
    }

    m_Screens.back()->Render(renderer);
}