#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>

struct Settings
{
    bool fullscreen = false;
    bool vsync = true;
    int resWidth = 1920;
    int resHeight = 1080;

    // Marks whether settings have changed and need applying to SDL.
    bool dirty = false;

    static std::string DefaultPath()
    {
        if (const char* home = std::getenv("HOME"))
        {
            return std::string(home) + "/raindropos/settings.json";
        }
        return "settings.json";
    }

    void Save() const
    {
        nlohmann::json j;
        j["fullscreen"] = fullscreen;
        j["vsync"] = vsync;
        j["resWidth"] = resWidth;
        j["resHeight"] = resHeight;

        std::ofstream f(DefaultPath());
        if (f.is_open())
        {
            f << j.dump(4);
            std::cout << "Settings saved to " << DefaultPath() << "\n";
        }
        else
        {
            std::cerr << "Settings: failed to save to " << DefaultPath() << "\n";
        }
    }

    static Settings Load()
    {
        Settings s;
        std::ifstream f(DefaultPath());
        if (!f.is_open())
        {
            std::cout << "Settings: no saved settings found, using defaults\n";
            
            return s;
        }

        try
        {
            nlohmann::json j = nlohmann::json::parse(f);
            s.fullscreen = j.value("fullscreen", s.fullscreen);
            s.vsync      = j.value("vsync",      s.vsync);
            s.resWidth   = j.value("resWidth",   s.resWidth);
            s.resHeight  = j.value("resHeight",  s.resHeight);
            std::cout << "Settings loaded from " << DefaultPath() << "\n";
        }
        catch (const nlohmann::json::exception& e)
        {
            std::cerr << "Settings: parse error — " << e.what() << ", using defaults\n";
        }

        return s;
    }
};