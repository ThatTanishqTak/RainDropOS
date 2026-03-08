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
    pid_t pid = fork();

    if (pid == 0)
    {
        if (m_Game.launch == LaunchType::Steam)
        {
            std::string uri = "steam://rungameid/" + std::to_string(m_Game.steamAppId);
            const char* argv[] = { "steam", uri.c_str(), nullptr };
            // execvp signature takes char* const* — the cast is required by
            // POSIX's historical API despite argv contents never being modified.
            execvp("steam", const_cast<char* const*>(argv));

            // Use write() instead of std::cerr — C++ iostream buffers are shared
            // after fork() and are not async-signal-safe before _exit().
            const char msg[] = "Failed to launch Steam (is steam in PATH?)\n";
            write(STDERR_FILENO, msg, sizeof(msg) - 1);
        }
        else
        {
            const char* argv[] = { m_Game.nativePath.c_str(), nullptr };
            execvp(m_Game.nativePath.c_str(), const_cast<char* const*>(argv));

            const char msg[] = "Failed to exec native binary\n";
            write(STDERR_FILENO, msg, sizeof(msg) - 1);
        }

        _exit(1);
    }
    else if (pid < 0)
    {
        // Still in the parent — std::cerr is safe here.
        std::cerr << "fork() failed\n";
    }
    // Parent returns immediately — the shell UI keeps running.
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
    SDL_Color headerBg = {  18,  20,  40, 255 };
    SDL_Color hintBg = {  18,  20,  40, 255 };
    SDL_Color borderColor = {  80, 120, 220, 255 };
    SDL_Color coverBg = {  28,  32,  58, 255 };
    SDL_Color optionBg = {  28,  32,  58, 255 };
    SDL_Color accent = {  80, 200, 140, 255 };

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

    // Launch URI or native path shown below the title
    std::string launchLabel = (m_Game.launch == LaunchType::Steam) ? "steam://rungameid/" + std::to_string(m_Game.steamAppId) : m_Game.nativePath;
    renderer.DrawText(launchLabel, 420, 188, 16, dimmed);

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