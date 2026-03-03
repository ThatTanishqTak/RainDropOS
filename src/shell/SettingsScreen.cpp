#include "SettingsScreen.h"
#include <iostream>

void SettingsScreen::OnEnter()
{
    m_Entries = { { "Fullscreen", "Off" }, { "Resolution", "1920x1080" }, { "VSync", "On" }, { "Exit Shell", "" }, };
    m_SelectedIndex = 0;
}

void SettingsScreen::ApplySelected()
{
    const std::string& label = m_Entries[m_SelectedIndex].label;

    if (label == "Fullscreen")
    {
        m_Entries[m_SelectedIndex].value = m_Entries[m_SelectedIndex].value == "Off" ? "On" : "Off";
    }
    else if (label == "VSync")
    {
        m_Entries[m_SelectedIndex].value = m_Entries[m_SelectedIndex].value == "On" ? "Off" : "On";
    }
    else if (label == "Resolution")
    {
        if (m_Entries[m_SelectedIndex].value == "1920x1080")
        {
            m_Entries[m_SelectedIndex].value = "1280x720";
        }
        else if (m_Entries[m_SelectedIndex].value == "1280x720")
        {
            m_Entries[m_SelectedIndex].value = "3840x2160";
        }
        else
        {
            m_Entries[m_SelectedIndex].value = "1920x1080";
        }
    }
    else if (label == "Exit Shell")
    {
        m_WantsToExit = true;
    }
}

void SettingsScreen::Update(Action action)
{
    int count = static_cast<int>(m_Entries.size());

    switch (action)
    {
        case Action::Up:
            m_SelectedIndex = (m_SelectedIndex - 1 + count) % count;
            break;

        case Action::Down:
            m_SelectedIndex = (m_SelectedIndex + 1) % count;
            break;

        case Action::Confirm:
        case Action::Right:
            ApplySelected();
            break;

        case Action::Back:
            m_WantsToExit = true;
            break;

        default:
            break;
    }
}

void SettingsScreen::Render(Renderer& renderer)
{
    renderer.Clear();

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color dimmed = { 140, 140, 150, 255 };
    SDL_Color headerBg = { 18, 20, 40, 255 };
    SDL_Color hintBg = { 18, 20, 40, 255 };
    SDL_Color borderColor = { 80, 120, 220, 255 };
    SDL_Color rowBg = { 28, 32, 58, 255 };
    SDL_Color accent = { 80, 200, 140, 255 };
    SDL_Color danger = { 220, 80, 80, 255 };

    // Header bar
    renderer.DrawRect(0, 0, 1920, 90, headerBg);
    renderer.DrawRect(0, 88, 1920, 2, borderColor);
    renderer.DrawText("RainDrop", 50, 22, 38, white);
    renderer.DrawText("SETTINGS", 330, 34, 18, dimmed);

    // Hint bar
    renderer.DrawRect(0, 1040, 1920, 40, hintBg);
    renderer.DrawRect(0, 1040, 1920, 1, borderColor);
    renderer.DrawText("Enter / A  Select      Right  Cycle      Esc / B  Back", 50, 1050, 15, dimmed);

    for (int i = 0; i < static_cast<int>(m_Entries.size()); i++)
    {
        int x = 80;
        int y = 120 + i * 70;

        bool isSelected = (i == m_SelectedIndex);
        bool isDanger = m_Entries[i].label == "Exit Shell";

        renderer.DrawRect(x, y, 900, 54, rowBg);
        renderer.DrawRectOutline(x, y, 900, 54, 2, isSelected ? borderColor : rowBg);
        renderer.DrawText(m_Entries[i].label, x + 24, y + 15, 20, isDanger ? danger : (isSelected ? white : dimmed));

        if (!m_Entries[i].value.empty())
        {
            renderer.DrawText(m_Entries[i].value, x + 700, y + 15, 20, isSelected ? accent : dimmed);
        }
    }

    renderer.Present();
}