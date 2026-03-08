#pragma once

#include "Screen.h"
#include "Settings.h"

#include <vector>
#include <string>

struct SettingsEntry
{
    std::string label;
    std::string value;
};

class SettingsScreen : public Screen
{
public:
    explicit SettingsScreen(Settings& settings);

    void OnEnter() override;
    void Update(Action action) override;
    void Render(Renderer& renderer) override;

private:
    void ApplySelected();

    Settings& m_Settings;
    std::vector<SettingsEntry>  m_Entries;
    int m_SelectedIndex = 0;

    static constexpr int START_X = 80;
    static constexpr int START_Y = 160;
    static constexpr int ROW_H   = 60;
};