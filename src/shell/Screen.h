#pragma once

#include "InputManager.h"
#include "Renderer.h"

#include <any>
#include <memory>

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void OnResume(std::any result = {}) {}

    virtual void Update(Action action) = 0;
    virtual void Render(Renderer& renderer) = 0;

    bool WantsToExit() const { return m_WantsToExit; }
    bool WantsToQuit() const { return m_WantsToQuit; }

    bool HasPendingPush() const { return m_PendingPush != nullptr; }
    std::unique_ptr<Screen> TakePendingPush() { return std::move(m_PendingPush); }

    std::any TakeResult() { return std::move(m_Result); }

    void ResetFlags()
    {
        m_WantsToExit = false;
        m_WantsToQuit = false;
        m_PendingPush = nullptr;
    }

protected:
    void SetResult(std::any result) { m_Result = std::move(result); }

    bool m_WantsToExit = false;
    bool m_WantsToQuit = false;
    std::unique_ptr<Screen> m_PendingPush = nullptr;

private:
    std::any m_Result;
};