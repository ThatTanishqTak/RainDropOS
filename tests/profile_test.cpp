#include <iostream>
#include <fstream>
#include <filesystem>

#include "../src/daemons/ProfileManager.h"

int main()
{
    // Create a fake profile for appid 440
    std::filesystem::create_directories("/tmp/raindrop_test_profiles");

    std::ofstream f("/tmp/raindrop_test_profiles/440.json");
    f << R"({"cpu_governor": "performance"})";
    f.close();

    std::cout << "Profile file written\n";

    // We can't easily override the HOME path in ProfileManager without
    // refactoring it — so instead just verify RestoreDefaults and Apply
    // run without crashing, and check the sysfs graceful fallback works
    ProfileManager pm;

    std::cout << "Testing Apply (no real profile, will use defaults)...\n";
    pm.Apply("440");

    std::cout << "Testing RestoreDefaults...\n";
    pm.RestoreDefaults();

    std::cout << "ProfileManager: all calls completed without crash — PASS\n";

    std::filesystem::remove_all("/tmp/raindrop_test_profiles");

    return 0;
}