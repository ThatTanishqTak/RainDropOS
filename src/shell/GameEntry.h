#pragma once

#include <string>

struct GameEntry
{
    std::string title;
    std::string executable;
    std::string appid;
    bool isSteam = false;
};