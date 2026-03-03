#pragma once

#include "InputManager.h"
#include "Renderer.h"

#include <memory>

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void OnEnter() {}
    virtual void OnExit()  {}

    virtual void Update(Action action) = 0;
    virtual void Render(Renderer& renderer) = 0;

    bool WantsToExit() const { return m_WantsToExit; }

    bool HasPendingPush() const { return m_PendingPush != nullptr; }
    std::unique_ptr<Screen> TakePendingPush() { return std::move(m_PendingPush); }

protected:
    bool m_WantsToExit = false;
    std::unique_ptr<Screen> m_PendingPush = nullptr;
};