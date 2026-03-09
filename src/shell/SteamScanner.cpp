#include "SteamScanner.h"
#include "VdfParser.h"

#include <filesystem>
#include <iostream>
#include <cstdlib>

std::vector<GameEntry> SteamScanner::Scan()
{
    const char* home = std::getenv("HOME");
    if (!home)
    {
        std::cerr << "SteamScanner: HOME not set\n";

        return {};
    }

    std::string steamRoot = std::string(home) + "/.steam/steam";
    std::cout << "SteamScanner: scanning " << steamRoot << "\n";

    return ScanPath(steamRoot);
}

std::vector<GameEntry> SteamScanner::ScanPath(const std::string& steamRoot)
{
    std::vector<GameEntry> games;
    auto libraries = FindLibraryPaths(steamRoot);

    std::cout << "SteamScanner: found " << libraries.size() << " library paths\n";

    for (const auto& lib : libraries)
    {
        ScanLibrary(lib, games);
    }

    std::cout << "SteamScanner: found " << games.size() << " installed games\n";

    return games;
}

std::vector<std::string> SteamScanner::FindLibraryPaths(const std::string& steamRoot)
{
    std::vector<std::string> paths;
    std::string vdfPath = steamRoot + "/config/libraryfolders.vdf";

    VdfNode root = VdfParser::ParseFile(vdfPath);
    if (root.children.empty())
    {
        std::cerr << "SteamScanner: could not parse libraryfolders.vdf (Steam may not be installed)\n";

        return paths;
    }

    // Keys are lowercased at parse time by VdfParser (see issue 5 fix).
    const VdfNode& folders = root["libraryfolders"];
    for (const auto& [key, node] : folders.children)
    {
        const std::string& path = node["path"].value;
        if (!path.empty())
        {
            paths.push_back(path);
            std::cout << "SteamScanner: library at " << path << "\n";
        }
    }

    if (paths.empty())
    {
        paths.push_back(steamRoot);
    }

    return paths;
}

std::optional<int> SteamScanner::ParseInt(const std::string& s)
{
    try
    {
        std::size_t pos = 0;
        int result = std::stoi(s, &pos);

        // Reject strings with trailing garbage, e.g. "4abc"
        if (pos != s.size())
        {
            return std::nullopt;
        }

        return result;
    }
    catch (const std::exception&)
    {
        return std::nullopt;
    }
}

void SteamScanner::ScanLibrary(const std::string& libraryPath, std::vector<GameEntry>& out)
{
    std::string steamappsPath = libraryPath + "/steamapps";

    if (!std::filesystem::exists(steamappsPath))
    {
        std::cerr << "SteamScanner: steamapps not found at " << steamappsPath << "\n";

        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(steamappsPath))
    {
        const std::string filename = entry.path().filename().string();

        if (filename.rfind("appmanifest_", 0) != 0)
        {
            continue;
        }

        if (entry.path().extension() != ".acf")
        {
            continue;
        }

        VdfNode root = VdfParser::ParseFile(entry.path().string());
        const VdfNode& appState = root["appstate"];

        if (appState.children.empty())
        {
            continue;
        }

        std::string stateFlags = appState["stateflags"].value;
        if (stateFlags.empty())
        {
            continue;
        }

        auto state = ParseInt(stateFlags);
        if (!state.has_value() || (state.value() & STATE_FULLY_INSTALLED) == 0)
        {
            if (!state.has_value())
            {
                std::cerr << "SteamScanner: invalid StateFlags value \"" << stateFlags << "\" in " << entry.path() << ", skipping\n";
            }
            continue;
        }

        std::string appidStr = appState["appid"].value;
        std::string name = appState["name"].value;

        if (appidStr.empty() || name.empty())
        {
            continue;
        }

        auto appidVal = ParseInt(appidStr);
        if (!appidVal.has_value() || appidVal.value() <= 0)
        {
            std::cerr << "SteamScanner: invalid appid \"" << appidStr << "\" in " << entry.path() << ", skipping\n";
            continue;
        }

        GameEntry game;
        game.title = name;
        game.launch = LaunchType::Steam;
        game.steamAppId = static_cast<uint32_t>(appidVal.value());

        out.push_back(game);
        std::cout << "SteamScanner: found " << name << " (appid " << game.steamAppId << ")\n";
    }
}