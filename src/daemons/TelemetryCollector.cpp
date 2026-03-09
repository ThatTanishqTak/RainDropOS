#include "TelemetryCollector.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

void TelemetryCollector::Update()
{
    m_Latest.cpu_percent  = ReadCpuPercent();
    m_Latest.temp_celsius = ReadTemperature();
    m_Latest.ram_used_mb  = ReadRamUsedMb();
}

float TelemetryCollector::ReadCpuPercent()
{
    std::ifstream file("/proc/stat");
    if (!file.is_open())
    {
        return 0.0f;
    }

    // First line: cpu user nice system idle iowait irq softirq
    std::string label;
    long user, nice, system, idle, iowait, irq, softirq;

    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);
    ss >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

    long total = user + nice + system + idle + iowait + irq + softirq;
    long totalDelta = total - m_PrevTotal;
    long idleDelta  = idle  - m_PrevIdle;

    m_PrevTotal = total;
    m_PrevIdle  = idle;

    if (totalDelta == 0)
    {
        return 0.0f;
    }

    return (1.0f - (float)idleDelta / (float)totalDelta) * 100.0f;
}

float TelemetryCollector::ReadTemperature()
{
    if (!std::filesystem::exists("/sys/class/hwmon"))
    {
        return 0.0f;
    }

    for (const auto& hwmon : std::filesystem::directory_iterator("/sys/class/hwmon"))
    {
        std::string tempPath = hwmon.path().string() + "/temp1_input";

        std::ifstream f(tempPath);
        if (!f.is_open())
        {
            continue;
        }

        long millidegrees = 0;
        f >> millidegrees;

        return millidegrees / 1000.0f;
    }

    return 0.0f;
}

float TelemetryCollector::ReadRamUsedMb()
{
    std::ifstream file("/proc/meminfo");
    if (!file.is_open())
    {
        return 0.0f;
    }

    long totalKb     = 0;
    long availableKb = 0;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("MemTotal:", 0) == 0)
        {
            std::istringstream ss(line);
            std::string label;
            ss >> label >> totalKb;
        }
        else if (line.rfind("MemAvailable:", 0) == 0)
        {
            std::istringstream ss(line);
            std::string label;
            ss >> label >> availableKb;
        }
    }

    return (totalKb - availableKb) / 1024.0f;
}