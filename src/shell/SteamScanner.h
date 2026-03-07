#pragma once

#include "GameEntry.h"

#include <vector>
#include <string>

class SteamScanner
{
public:
    // Returns all installed Steam games found on the system
    std::vector<GameEntry> Scan();
    std::vector<GameEntry> ScanPath(const std::string& steamRoot);

private:
    std::vector<std::string> FindLibraryPaths();
    void ScanLibrary(const std::string& path, std::vector<GameEntry>& out);

    static constexpr const char* STEAM_ROOT = "/home/raindrop/.steam/steam";
    static constexpr int STATE_FULLY_INSTALLED = 4;
};