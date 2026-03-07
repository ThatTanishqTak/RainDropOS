#include "SteamScanner.h"
#include "VdfParser.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>

std::vector<GameEntry> SteamScanner::Scan()
{
    // Get home directory at runtime
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
        std::cerr << "SteamScanner: could not parse libraryfolders.vdf" << " (Steam may not be installed)\n";
        return paths; // return empty — no Steam installed
    }

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
        const VdfNode& appState = root["AppState"];

        if (appState.children.empty())
        {
            continue;
        }

        std::string stateFlags = appState["StateFlags"].value;
        if (stateFlags.empty())
        {
            continue;
        }

        int state = std::stoi(stateFlags);
        if ((state & STATE_FULLY_INSTALLED) == 0)
        {
            continue;
        }

        std::string appid = appState["appid"].value;
        std::string name  = appState["name"].value;

        if (appid.empty() || name.empty())
        {
            continue;
        }

        GameEntry game;
        game.title = name;
        game.appid = appid;
        game.isSteam = true;
        game.executable = "steam steam://rungameid/" + appid;

        out.push_back(game);
        std::cout << "SteamScanner: found " << name << " (appid " << appid << ")\n";
    }
}