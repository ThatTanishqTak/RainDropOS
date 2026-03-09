#pragma once

#include "Screen.h"
#include "GameEntry.h"
#include "IpcClient.h"

enum class DetailAction
{
    Launch,
    Back,
};

class GameDetailScreen : public Screen
{
public:
    explicit GameDetailScreen(const GameEntry& game);

    void OnEnter() override;
    void Update(Action action) override;
    void Render(Renderer& renderer) override;

private:
    void LaunchGame();

    GameEntry m_Game;
    IpcClient m_Ipc;
    int m_SelectedIndex = 0;

    static constexpr int OPTION_COUNT = 2;
};