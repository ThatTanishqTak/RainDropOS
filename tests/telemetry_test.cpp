#include <iostream>
#include "../src/daemons/TelemetryCollector.h"

int main()
{
    TelemetryCollector collector;

    // First update seeds the CPU counters
    collector.Update();

    // Second update gives a meaningful CPU delta
    collector.Update();

    TelemetryData data = collector.GetLatest();

    std::cout << "CPU:  " << data.cpu_percent  << "%\n";
    std::cout << "Temp: " << data.temp_celsius << "C\n";
    std::cout << "RAM:  " << data.ram_used_mb  << "MB\n";

    // We can't assert exact values — just verify nothing is obviously broken
    bool cpuOk  = data.cpu_percent  >= 0.0f && data.cpu_percent  <= 100.0f;
    bool ramOk  = data.ram_used_mb  > 0.0f;

    std::cout << "CPU range valid: " << (cpuOk ? "PASS" : "FAIL") << "\n";
    std::cout << "RAM reading valid: " << (ramOk ? "PASS" : "FAIL") << "\n";

    // Temperature may be 0 in WSL2 — that's expected
    std::cout << "Temp (0 is OK in WSL2): " << data.temp_celsius << "C\n";

    return (cpuOk && ramOk) ? 0 : 1;
}