#pragma once

#include "Screen.h"

class LibraryScreen : public Screen
{
public:
    void OnEnter() override;
    void Update(Action action) override;
    void Render(Renderer& renderer) override;
};