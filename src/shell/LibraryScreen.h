#pragma once

#include "Screen.h"
#include "GameEntry.h"

#include <vector>
#include <string>

class LibraryScreen : public Screen
{
public:
    void OnEnter() override;
    void Update(Action action) override;
    void Render(Renderer& renderer) override;

private:
    void LoadLibrary();

private:
    std::vector<GameEntry> m_Games;
    int m_SelectedIndex = 0;
    std::string m_LastError;

    static constexpr int TILE_W = 200;
    static constexpr int TILE_H = 120;
    static constexpr int TILE_GAP = 20;
    static constexpr int TILE_START_X = 50;
    static constexpr int TILE_START_Y = 160;
    static constexpr int TILES_PER_ROW = 5;
};