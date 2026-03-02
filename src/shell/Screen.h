#pragma once

#include "InputManager.h"
#include "Renderer.h"

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void OnEnter() {}
    virtual void OnExit()  {}

    virtual void Update(Action action) = 0;
    virtual void Render(Renderer& renderer) = 0;

    bool WantsToExit() const { return m_WantsToExit; }

protected:
    bool m_WantsToExit = false;
};