#pragma once

#include <string>
#include <cstdint>

enum class LaunchType
{
    Native,
    Steam
};

struct GameEntry
{
    std::string title;
    LaunchType  launch = LaunchType::Native;

    std::string nativePath;
    uint32_t steamAppId = 0; 
};