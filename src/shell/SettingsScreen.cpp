#include "SettingsScreen.h"
#include <iostream>

SettingsScreen::SettingsScreen(Settings& settings) : m_Settings(settings)
{

}

void SettingsScreen::OnEnter()
{
    // Populate entries from live Settings so they reflect the saved state.
    m_Entries =
    {
        { "Fullscreen", m_Settings.fullscreen ? "On" : "Off" },
        { "Resolution", std::to_string(m_Settings.resWidth) + "x" + std::to_string(m_Settings.resHeight) },
        { "VSync", m_Settings.vsync ? "On" : "Off" },
        { "Exit Shell", "" },
    };

    m_SelectedIndex = 0;
}

void SettingsScreen::ApplySelected()
{
    const std::string& label = m_Entries[m_SelectedIndex].label;

    if (label == "Fullscreen")
    {
        m_Settings.fullscreen = !m_Settings.fullscreen;
        m_Entries[m_SelectedIndex].value = m_Settings.fullscreen ? "On" : "Off";
        m_Settings.dirty = true;
        m_Settings.Save();
    }
    else if (label == "VSync")
    {
        m_Settings.vsync = !m_Settings.vsync;
        m_Entries[m_SelectedIndex].value = m_Settings.vsync ? "On" : "Off";
        // VSync requires renderer recreation — applied on next launch.
        std::cout << "VSync change will take effect on next launch.\n";
        m_Settings.Save();
    }
    else if (label == "Resolution")
    {
        // Cycle through common resolutions.
        if (m_Settings.resWidth == 1920)
        {
            m_Settings.resWidth = 1280;
            m_Settings.resHeight = 720; 
        }
        else if (m_Settings.resWidth == 1280)
        {
            m_Settings.resWidth = 3840;
            m_Settings.resHeight = 2160;
        }
        else
        {
            m_Settings.resWidth = 1920;
            m_Settings.resHeight = 1080;
        }

        m_Entries[m_SelectedIndex].value = std::to_string(m_Settings.resWidth) + "x" + std::to_string(m_Settings.resHeight);
        m_Settings.dirty = true;
        m_Settings.Save();
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