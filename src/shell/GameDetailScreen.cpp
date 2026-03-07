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
    std::cout << "Launching: " << m_Game.title << "\n";

    if (m_Game.isSteam)
    {
        // Launch via Steam URI
        std::string cmd = "steam steam://rungameid/" + m_Game.appid + " &";
        system(cmd.c_str());
        
        return;
    }

    // Native launch via fork/exec
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
    SDL_Color dimmed = { 140, 140, 150, 255 };
    SDL_Color headerBg = { 18, 20, 40, 255 };
    SDL_Color hintBg = { 18, 20, 40, 255 };
    SDL_Color borderColor = { 80, 120, 220, 255 };
    SDL_Color coverBg = { 28, 32, 58, 255 };
    SDL_Color optionBg = { 28, 32, 58, 255 };
    SDL_Color accent = { 80, 200, 140, 255 };

    // Header bar
    renderer.DrawRect(0, 0, 1920, 90, headerBg);
    renderer.DrawRect(0, 88, 1920, 2, borderColor);
    renderer.DrawText("RainDrop", 50, 22, 38, white);
    renderer.DrawText(m_Game.title, 330, 34, 18, dimmed);

    // Hint bar
    renderer.DrawRect(0, 1040, 1920, 40, hintBg);
    renderer.DrawRect(0, 1040, 1920, 1, borderColor);
    renderer.DrawText("Enter / A  Select      Esc / B  Back", 50, 1050, 15, dimmed);

    // Cover art placeholder
    renderer.DrawRect(80, 120, 300, 380, coverBg);
    renderer.DrawRectOutline(80, 120, 300, 380, 2, borderColor);
    renderer.DrawText("NO ART", 175, 295, 18, dimmed);

    // Game title
    renderer.DrawText(m_Game.title, 420, 120, 40, white);

    // Divider
    renderer.DrawRect(420, 175, 800, 2, borderColor);

    // Executable path
    renderer.DrawText(m_Game.executable, 420, 188, 16, dimmed);

    // Options
    const char* options[OPTION_COUNT] = { "Launch", "Back" };

    for (int i = 0; i < OPTION_COUNT; i++)
    {
        int x = 420;
        int y = 260 + i * 70;

        bool isSelected = (i == m_SelectedIndex);

        renderer.DrawRect(x, y, 280, 52, optionBg);
        renderer.DrawRectOutline(x, y, 280, 52, 2, isSelected ? borderColor : optionBg);
        renderer.DrawText(options[i], x + 20, y + 14, 20, isSelected ? accent : dimmed);
    }

    renderer.Present();
}