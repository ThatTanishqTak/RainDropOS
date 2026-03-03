#include "LibraryScreen.h"
#include "GameDetailScreen.h"
#include "SettingsScreen.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void LibraryScreen::OnEnter()
{
    LoadLibrary();
    m_SelectedIndex = 0;
}

void LibraryScreen::LoadLibrary()
{
    m_Games.clear();

    std::ifstream file("library.json");
    if (!file.is_open())
    {
        std::cerr << "Could not open library.json\n";

        return;
    }

    try
    {
        json data = json::parse(file);

        for (const auto& entry : data["games"])
        {
            GameEntry game;
            game.title = entry["title"].get<std::string>();
            game.executable = entry["executable"].get<std::string>();
            m_Games.push_back(game);
        }

        std::cout << "Loaded " << m_Games.size() << " games from library.json\n";
    }
    catch (const json::exception& e)
    {
        std::cerr << "Failed to parse library.json: " << e.what() << "\n";
    }
}

void LibraryScreen::Update(Action action)
{
    int count = static_cast<int>(m_Games.size());
    if (count == 0)
    {
        return;
    }

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

        case Action::Confirm:
            if (!m_Games.empty())
            {
                m_PendingPush = std::make_unique<GameDetailScreen>(m_Games[m_SelectedIndex]);
            }
            break;

        case Action::Menu:
            m_PendingPush = std::make_unique<SettingsScreen>();
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
    SDL_Color noGames = { 200,  80,  80, 255 };
    SDL_Color errorCol = { 220, 100,  80, 255 };

    renderer.DrawText("RainDrop", 50, 30, 42, white);
    renderer.DrawText("MY LIBRARY", 50, 110, 18, dimmed);

    if (!m_LastError.empty())
    {
        renderer.DrawText(m_LastError, 50, 680, 16, errorCol);
    }

    if (m_Games.empty())
    {
        renderer.DrawText("No games found. Add entries to library.json", 50, 200, 20, noGames);
        renderer.Present();
    
        return;
    }

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

    renderer.DrawText("Tab / Start  Settings      Esc / B  Exit", 50, 660, 16, dimmed);

    renderer.Present();
}