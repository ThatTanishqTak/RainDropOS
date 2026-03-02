#pragma once

#include "Screen.h"

#include <vector>
#include <memory>

class ScreenStack
{
public:
    void Push(std::unique_ptr<Screen> screen);
    void Pop();

    void Update(Action action);
    void Render(Renderer& renderer);

    bool IsEmpty() const { return m_Screens.empty(); }

private:
    std::vector<std::unique_ptr<Screen>> m_Screens;
};