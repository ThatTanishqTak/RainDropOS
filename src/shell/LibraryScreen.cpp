#include "LibraryScreen.h"
#include "GameDetailScreen.h"
#include "SettingsScreen.h"
#include "SteamScanner.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

LibraryScreen::LibraryScreen(Settings& settings) : m_Settings(settings)
{

}

void LibraryScreen::OnEnter()
{
    LoadLibrary();
    m_SelectedIndex = 0;
}

void LibraryScreen::OnResume(std::any result)
{
    // If returning from SettingsScreen, the updated Settings have already
    // been mutated in-place and saved. Nothing more to do here — Application
    // will spot the dirty flag and apply SDL changes on the next frame.
}

void LibraryScreen::LoadLibrary()
{
    m_Games.clear();
    m_LastError.clear();

    // Build the list of candidate paths. The home-directory path is only
    // added when HOME is actually set — constructing std::string from a
    // null pointer is undefined behaviour.
    std::vector<std::string> searchPaths = { "library.json" };

    if (const char* home = std::getenv("HOME"))
    {
        searchPaths.push_back(std::string(home) + "/raindropos/library.json");
    }
    else
    {
        std::cerr << "LibraryScreen: HOME is not set, skipping home-directory search path\n";
    }

    std::ifstream file;

    for (const auto& path : searchPaths)
    {
        // clear() is required before re-opening a stream that previously
        // failed — without it the failbit stays set and the new open is ignored.
        file.clear();
        file.open(path);
        if (file.is_open())
        {
            std::cout << "Loading library from: " << path << "\n";
            
            break;
        }
    }

    if (file.is_open())
    {
        try
        {
            nlohmann::json data = nlohmann::json::parse(file);

            for (const auto& entry : data["games"])
            {
                GameEntry game;
                game.title = entry["title"].get<std::string>();
                game.launch = LaunchType::Native;
                game.nativePath = entry["executable"].get<std::string>();
                
                m_Games.push_back(game);
            }

            std::cout << "Loaded " << m_Games.size() << " manual games from library.json\n";
        }
        catch (const nlohmann::json::exception& e)
        {
            m_LastError = std::string("library.json parse error: ") + e.what();
            std::cerr << m_LastError << "\n";
        }
    }
    else
    {
        // Not an error worth surfacing — library.json is optional if Steam games exist.
        std::cout << "No library.json found, continuing with Steam scan\n";
    }

    // Scan for Steam games
    SteamScanner scanner;
    auto steamGames = scanner.Scan();
    for (auto& game : steamGames)
    {
        m_Games.push_back(game);
    }

    std::sort(m_Games.begin(), m_Games.end(),
        [](const GameEntry& a, const GameEntry& b)
        {
            return a.title < b.title;
        });

    if (m_Games.empty() && m_LastError.empty())
    {
        m_LastError = "No games found. Add entries to library.json or install Steam games.";
    }

    std::cout << "Total games in library: " << m_Games.size() << "\n";
}

void LibraryScreen::Update(Action action)
{
    // Back and Menu must always be reachable — even with an empty library
    // the user needs a way to reach settings or exit the shell.
    switch (action)
    {
        case Action::Menu:
            m_PendingPush = std::make_unique<SettingsScreen>(m_Settings);
            return;

        case Action::Back:
            m_WantsToExit = true;
            return;

        default:
            break;
    }

    // Everything below requires at least one game to act on.
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
            m_PendingPush = std::make_unique<GameDetailScreen>(m_Games[m_SelectedIndex]);
            break;

        default:
            break;
    }
}

void LibraryScreen::Render(Renderer& renderer)
{
    renderer.Clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color dimmed = { 140, 140, 150, 255 };
    SDL_Color tileBg = {  28,  32,  58, 255 };
    SDL_Color headerBg = {  18,  20,  40, 255 };
    SDL_Color hintBg = {  18,  20,  40, 255 };
    SDL_Color borderColor = {  80, 120, 220, 255 };
    SDL_Color noGames = { 200,  80,  80, 255 };
    SDL_Color errorCol = { 220, 100,  80, 255 };

    // Header bar
    renderer.DrawRect(0, 0, 1920, 90, headerBg);
    renderer.DrawRect(0, 88, 1920, 2, borderColor);
    renderer.DrawText("RainDrop", 50, 22, 38, white);
    renderer.DrawText("MY LIBRARY", 330, 34, 18, dimmed);

    // Hint bar
    renderer.DrawRect(0, 1040, 1920, 40, hintBg);
    renderer.DrawRect(0, 1040, 1920, 1, borderColor);
    renderer.DrawText("Enter / A  Open      Tab / Start  Settings      Esc / B  Exit", 50, 1050, 15, dimmed);

    if (!m_LastError.empty())
    {
        renderer.DrawText(m_LastError, 50, 1010, 15, errorCol);
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

        renderer.DrawRect(x, y, TILE_W, TILE_H, tileBg);

        if (isSelected)
        {
            renderer.DrawRectOutline(x, y, TILE_W, TILE_H, 3, borderColor);
        }

        std::string label = m_Games[i].title + (m_Games[i].launch == LaunchType::Steam ? " [S]" : "");
        renderer.DrawText(label, x, y + TILE_H + 6, 16, isSelected ? white : dimmed);
    }

    renderer.Present();
}