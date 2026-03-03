#include "GameDetailScreen.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

GameDetailScreen::GameDetailScreen(const GameEntry& game) : m_Game(game)
{

}

void GameDetailScreen::OnEnter()
{
    m_SelectedIndex = 0;
}

void GameDetailScreen::LaunchGame()
{
    std::cout << "Launching: " << m_Game.title << " (" << m_Game.executable << ")\n";

    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(m_Game.executable.c_str(), nullptr);
        std::cerr << "Failed to launch: " << m_Game.executable << "\n";
        _exit(1);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        std::cout << "Game exited, returning to shell\n";
    }
    else
    {
        std::cerr << "fork() failed\n";
    }
}

void GameDetailScreen::Update(Action action)
{
    switch (action)
    {
        case Action::Up:
            m_SelectedIndex = (m_SelectedIndex - 1 + OPTION_COUNT) % OPTION_COUNT;
            break;

        case Action::Down:
            m_SelectedIndex = (m_SelectedIndex + 1) % OPTION_COUNT;
            break;

        case Action::Confirm:
            if (m_SelectedIndex == 0)
            {
                LaunchGame();
            }
            else
            {
                m_WantsToExit = true;
            }
            break;

        case Action::Back:
            m_WantsToExit = true;
            break;

        default:
            break;
    }
}

void GameDetailScreen::Render(Renderer& renderer)
{
    renderer.Clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color dimmed = { 160, 160, 160, 255 };
    SDL_Color highlight = { 80, 120, 220, 255 };
    SDL_Color optionBg = { 30, 35, 60, 255 };
    SDL_Color selectedBg = { 50, 65, 120, 255 };
    SDL_Color accent = { 80, 200, 140, 255 };

    // Cover art placeholder
    SDL_Color coverBg = { 40, 50, 90, 255 };
    renderer.DrawRect(80, 80, 280, 360, coverBg);
    renderer.DrawText("NO ART", 155, 245, 18, dimmed);

    // Title and metadata
    renderer.DrawText(m_Game.title, 400, 80, 36, white);
    renderer.DrawText(m_Game.executable, 400, 135, 16, dimmed);

    // Divider line
    renderer.DrawRect(400, 170, 700, 2, dimmed);

    // Options
    const char* options[OPTION_COUNT] = { "Launch", "Back" };

    for (int i = 0; i < OPTION_COUNT; i++)
    {
        int x = 400;
        int y = 200 + i * 70;

        bool isSelected = (i == m_SelectedIndex);

        renderer.DrawRect(x, y, 300, 50, isSelected ? selectedBg : optionBg);
        renderer.DrawText(options[i], x + 20, y + 13, 20, isSelected ? accent : dimmed);
    }

    renderer.DrawText("A / Enter  Select      B / Esc  Back", 80, 660, 16, dimmed);

    renderer.Present();
}