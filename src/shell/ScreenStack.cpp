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

        std::any result = m_Screens.back()->TakeResult();
        m_Screens.pop_back();

        if (!m_Screens.empty())
        {
            m_Screens.back()->OnResume(std::move(result));
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

    const bool wantsQuit = top->WantsToQuit();
    const bool wantsExit = top->WantsToExit();
    std::unique_ptr<Screen> pending = top->HasPendingPush() ? top->TakePendingPush() : nullptr;

    top->ResetFlags();

    // Priority: push > quit > exit
    if (pending)
    {
        Push(std::move(pending));
    }
    else if (wantsQuit)
    {
        m_WantsToQuit = true;
    }
    else if (wantsExit)
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