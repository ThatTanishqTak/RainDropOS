#include "SettingsScreen.h"
#include <iostream>

void SettingsScreen::OnEnter()
{
    m_Entries = { { "Fullscreen", "Off" }, { "Resolution", "1920x1080" }, { "VSync", "On" }, { "Exit Shell",  "" }, };
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
    SDL_Color dimmed = { 160, 160, 160, 255 };
    SDL_Color highlight = { 80, 120, 220, 255 };
    SDL_Color rowBg = { 30,  35,  60, 255 };
    SDL_Color selectedBg = { 50,  65, 120, 255 };
    SDL_Color accent = { 80, 200, 140, 255 };
    SDL_Color danger = { 220,  80,  80, 255 };

    renderer.DrawText("RainDrop", 80, 30, 42, white);
    renderer.DrawText("SETTINGS", 80, 110, 18, dimmed);

    for (int i = 0; i < static_cast<int>(m_Entries.size()); i++)
    {
        int x = START_X;
        int y = START_Y + i * ROW_H;

        bool isSelected = (i == m_SelectedIndex);
        renderer.DrawRect(x, y, 800, ROW_H - 6, isSelected ? selectedBg : rowBg);

        bool isDanger = m_Entries[i].label == "Exit Shell";
        renderer.DrawText(m_Entries[i].label, x + 20, y + 14, 20, isDanger ? danger : (isSelected ? white : dimmed));

        if (!m_Entries[i].value.empty())
        {
            renderer.DrawText(m_Entries[i].value, x + 600, y + 14, 20, isSelected ? accent : dimmed);
        }
    }

    renderer.DrawText("B / Esc  Back", 80, 660, 16, dimmed);

    renderer.Present();
}