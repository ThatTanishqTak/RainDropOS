#include <iostream>
#include "../src/shell/VdfParser.h"

int main()
{
    VdfNode root = VdfParser::ParseFile("tests/test.vdf");

    std::string name  = root["AppState"]["name"].value;
    std::string appid = root["AppState"]["appid"].value;
    std::string lang  = root["AppState"]["UserConfig"]["language"].value;

    std::cout << "Name:     " << name  << "\n";
    std::cout << "AppID:    " << appid << "\n";
    std::cout << "Language: " << lang  << "\n";

    if (name == "Team Fortress 2" && appid == "440" && lang == "english")
    {
        std::cout << "All tests passed.\n";
        return 0;
    }
    else
    {
        std::cout << "Test FAILED.\n";
        return 1;
    }
}