#pragma once

#include <string>

class ProfileManager
{
public:
    // Apply the profile for a given appid — loads from disk, falls back to defaults
    void Apply(const std::string& appid);

    // Restore system defaults — called when a game exits
    void RestoreDefaults();

private:
    std::string LoadGovernor(const std::string& appid);
    void SetCpuGovernor(const std::string& governor);

    static constexpr const char* DEFAULT_GOVERNOR = "ondemand";
};