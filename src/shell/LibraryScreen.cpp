#include "LibraryScreen.h"

void LibraryScreen::OnEnter()
{
    m_Games =
    {
        { "Game One" },
        { "Game Two" },
        { "Game Three" },
        { "Game Four" },
        { "Game Five" },
        { "Game Six" },
        { "Game Seven" },
        { "Game Eight" },
    };

    m_SelectedIndex = 0;
}

void LibraryScreen::Update(Action action)
{
    int count = static_cast<int>(m_Games.size());

    switch (action)
    {
        case Action::Right:
            m_SelectedIndex = (m_SelectedIndex + 1) % count;
            break;

        case Action::Left:
            m_SelectedIndex = (m_SelectedIndex - 1 + count) % count;
            break;

        case Action::Down:
            if (m_SelectedIndex + TILES_PER_ROW < count)
            {
                m_SelectedIndex += TILES_PER_ROW;
            }
            break;

        case Action::Up:
            if (m_SelectedIndex - TILES_PER_ROW >= 0)
            {
                m_SelectedIndex -= TILES_PER_ROW;
            }
            break;

        case Action::Back:
            m_WantsToExit = true;
            break;

        default:
            break;
    }
}

void LibraryScreen::Render(Renderer& renderer)
{
    renderer.Clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color tile = {  40,  50,  90, 255 };
    SDL_Color highlight = {  80, 120, 220, 255 };
    SDL_Color dimmed = { 160, 160, 160, 255 };

    renderer.DrawText("RainDrop", 50, 30, 42, white);
    renderer.DrawText("MY LIBRARY", 50, 110, 18, dimmed);

    for (int i = 0; i < static_cast<int>(m_Games.size()); i++)
    {
        int col = i % TILES_PER_ROW;
        int row = i / TILES_PER_ROW;

        int x = TILE_START_X + col * (TILE_W + TILE_GAP);
        int y = TILE_START_Y + row * (TILE_H + TILE_GAP + 30);

        bool isSelected = (i == m_SelectedIndex);

        renderer.DrawRect(x, y, TILE_W, TILE_H, isSelected ? highlight : tile);
        renderer.DrawText(m_Games[i].title, x, y + TILE_H + 6, 16, isSelected ? white : dimmed);
    }

    renderer.Present();
}