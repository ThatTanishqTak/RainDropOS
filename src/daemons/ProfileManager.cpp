#include "ProfileManager.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <nlohmann/json.hpp>

void ProfileManager::Apply(const std::string& appid)
{
    std::string governor = LoadGovernor(appid);
    std::cout << "ProfileManager: applying governor '" << governor << "' for appid " << appid << "\n";
    SetCpuGovernor(governor);
}

void ProfileManager::RestoreDefaults()
{
    std::cout << "ProfileManager: restoring default governor '" << DEFAULT_GOVERNOR << "'\n";
    SetCpuGovernor(DEFAULT_GOVERNOR);
}

std::string ProfileManager::LoadGovernor(const std::string& appid)
{
    const char* home = std::getenv("HOME");
    if (!home)
    {
        return DEFAULT_GOVERNOR;
    }

    std::string path = std::string(home) + "/.config/raindrop/profiles/" + appid + ".nlohmann::json";

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "ProfileManager: no profile for appid " << appid << ", using defaults\n";

        return DEFAULT_GOVERNOR;
    }

    try
    {
        nlohmann::json j = nlohmann::json::parse(file);
        std::string governor = j.value("cpu_governor", std::string(DEFAULT_GOVERNOR));
        std::cout << "ProfileManager: loaded profile from " << path << "\n";

        return governor;
    }
    catch (const std::exception& e)
    {
        std::cerr << "ProfileManager: failed to parse " << path << " — " << e.what() << "\n";
        return DEFAULT_GOVERNOR;
    }
}

void ProfileManager::SetCpuGovernor(const std::string& governor)
{
    std::string cpuRoot = "/sys/devices/system/cpu";

    if (!std::filesystem::exists(cpuRoot))
    {
        std::cout << "ProfileManager: sysfs not available (WSL2?), skipping governor write\n";

        return;
    }

    int applied = 0;

    for (const auto& entry : std::filesystem::directory_iterator(cpuRoot))
    {
        std::string name = entry.path().filename().string();

        // Only cpu0, cpu1, cpu2 etc — skip cpufreq, cpuidle, cpumap etc
        if (name.rfind("cpu", 0) != 0)
        {
            continue;
        }

        if (name.size() < 4)
        {
            continue;
        }

        if (!std::isdigit((unsigned char)name[3]))
        {
            continue;
        }

        std::string govPath = entry.path().string() + "/cpufreq/scaling_governor";

        std::ofstream f(govPath);
        if (f.is_open())
        {
            f << governor;
            applied++;
        }
    }

    if (applied > 0)
    {
        std::cout << "ProfileManager: set governor to '" << governor << "' on " << applied << " cores\n";
    }
    else
    {
        std::cout << "ProfileManager: no cpufreq cores found — " << "governor not applied (expected on WSL2)\n";
    }
}