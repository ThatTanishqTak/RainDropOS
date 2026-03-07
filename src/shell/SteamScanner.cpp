#include "SteamScanner.h"
#include "VdfParser.h"
#include <filesystem>
#include <iostream>

std::vector<GameEntry> SteamScanner::Scan()
{
    std::vector<GameEntry> games;

    auto libraries = FindLibraryPaths();
    std::cout << "SteamScanner: found " << libraries.size() << " library paths\n";

    for (const auto& lib : libraries)
    {
        ScanLibrary(lib, games);
    }

    std::cout << "SteamScanner: found " << games.size() << " installed games\n";

    return games;
}

std::vector<std::string> SteamScanner::FindLibraryPaths()
{
    std::vector<std::string> paths;
    std::string vdfPath = std::string(STEAM_ROOT) + "/config/libraryfolders.vdf";

    VdfNode root = VdfParser::ParseFile(vdfPath);
    if (root.children.empty())
    {
        std::cerr << "SteamScanner: could not parse libraryfolders.vdf\n";
        // Fall back to default Steam path
        paths.push_back(STEAM_ROOT);

        return paths;
    }

    // libraryfolders.vdf structure:
    // "libraryfolders" { "0" { "path" "..." } "1" { "path" "..." } }
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
        paths.push_back(STEAM_ROOT);
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

        // Look for appmanifest_*.acf files
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

        // Only show fully installed games
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
        std::string name = appState["name"].value;

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

std::vector<GameEntry> SteamScanner::ScanPath(const std::string& steamRoot)
{
    std::vector<GameEntry> games;
    ScanLibrary(steamRoot, games);
    
    return games;
}