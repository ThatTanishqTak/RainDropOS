#include "LibraryScreen.h"

void LibraryScreen::OnEnter()
{
    
}

void LibraryScreen::Update(Action action)
{
    if (action == Action::Back)
    {
        m_WantsToExit = true;
    }
}

void LibraryScreen::Render(Renderer& renderer)
{
    renderer.Clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color blue  = {  60,  80, 140, 255 };

    renderer.DrawText("RainDrop Shell", 50, 40, 36, white);
    renderer.DrawText("My Library", 50, 100, 22, white);

    renderer.DrawRect(50, 140, 200, 120, blue);
    renderer.DrawText("Game Title", 60, 230, 18, white);

    renderer.Present();
}