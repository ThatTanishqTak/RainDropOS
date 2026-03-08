#pragma once

#include "GameEntry.h"

#include <vector>
#include <string>
#include <optional>

class SteamScanner
{
public:
    std::vector<GameEntry> Scan();
    std::vector<GameEntry> ScanPath(const std::string& steamRoot);

private:
    std::vector<std::string> FindLibraryPaths(const std::string& steamRoot);
    void ScanLibrary(const std::string& path, std::vector<GameEntry>& out);

    static std::optional<int> ParseInt(const std::string& s);

    static constexpr int STATE_FULLY_INSTALLED = 4;
};