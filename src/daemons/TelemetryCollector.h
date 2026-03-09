#pragma once

#include "../../include/IpcMessages.h"

class TelemetryCollector
{
public:
    // Call this on a timer — updates the stored telemetry snapshot
    void Update();

    TelemetryData GetLatest() const { return m_Latest; }

private:
    float ReadCpuPercent();
    float ReadTemperature();
    float ReadRamUsedMb();

    TelemetryData m_Latest;

    // Stored between Update() calls for CPU delta calculation
    long m_PrevIdle  = 0;
    long m_PrevTotal = 0;
};