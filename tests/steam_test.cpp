#include <iostream>
#include <fstream>
#include <filesystem>
#include "../src/shell/SteamScanner.h"

void CreateTestData()
{
    // Create fake Steam library structure
    std::filesystem::create_directories("tests/fake_steam/config");
    std::filesystem::create_directories("tests/fake_steam/steamapps");

    // libraryfolders.vdf
    std::ofstream lf("tests/fake_steam/config/libraryfolders.vdf");
    lf << R"(
"libraryfolders"
{
    "0"
    {
        "path"    "tests/fake_steam"
    }
}
)";
    lf.close();

    // appmanifest for TF2
    std::ofstream app("tests/fake_steam/steamapps/appmanifest_440.acf");
    app << R"(
"AppState"
{
    "appid"        "440"
    "name"         "Team Fortress 2"
    "StateFlags"   "4"
}
)";
    app.close();

    // appmanifest for a not-installed game (StateFlags != 4)
    std::ofstream app2("tests/fake_steam/steamapps/appmanifest_570.acf");
    app2 << R"(
"AppState"
{
    "appid"        "570"
    "name"         "Dota 2"
    "StateFlags"   "2"
}
)";
    app2.close();
}

int main()
{
    CreateTestData();

    // Point scanner at test data
    SteamScanner scanner;
    auto games = scanner.ScanPath("tests/fake_steam");

    std::cout << "Found " << games.size() << " games\n";
    for (const auto& g : games)
    {
        std::cout << "  " << g.title << " (appid " << g.steamAppId << ")\n";
    }

    if (games.size() == 1 && games[0].title == "Team Fortress 2")
    {
        std::cout << "All tests passed.\n";
        return 0;
    }

    std::cout << "Test FAILED.\n";
    return 1;
}