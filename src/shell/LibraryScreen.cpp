#include "LibraryScreen.h"
#include "SettingsScreen.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

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
            game.title      = entry["title"].get<std::string>();
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

void LibraryScreen::LaunchSelected()
{
    if (m_Games.empty()) { return; }

    const GameEntry& game = m_Games[m_SelectedIndex];
    std::cout << "Launching: " << game.title << " (" << game.executable << ")\n";

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process — replace itself with the game
        execvp(game.executable.c_str(), nullptr);

        // If we reach here execvp failed
        std::cerr << "Failed to launch: " << game.executable << "\n";
        _exit(1);
    }
    else if (pid > 0)
    {
        // Parent process — wait for game to exit
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            std::cerr << "Game exited with error code: " << WEXITSTATUS(status) << "\n";
            m_LastError = "Game failed to launch or exited with an error";
        }
        else
        {
            m_LastError.clear();
        }

        std::cout << "Game exited, returning to shell\n";
    }
    else
    {
        std::cerr << "fork() failed\n";
        m_LastError = "Failed to start game process";
    }
}

void LibraryScreen::Update(Action action)
{
    int count = static_cast<int>(m_Games.size());
    if (count == 0) { return; }

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

        case Action::Menu:
            m_PendingPush = std::make_unique<SettingsScreen>();
            break;

        case Action::Confirm:
            LaunchSelected();
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

    renderer.Present();
}