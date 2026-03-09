#pragma once

#include "IpcServer.h"
#include "TelemetryCollector.h"
#include "ProfileManager.h"
#include "../../include/IpcMessages.h"

class Daemon
{
public:
    bool Initialize();
    void Run();
    void Stop();

private:
    std::string HandleMessage(const IpcMessage& message);

    IpcServer m_Server;
    TelemetryCollector m_Telemetry;
    ProfileManager m_Profiles;
    bool m_Running = false;

    static constexpr const char* SOCKET_PATH    = "/tmp/raindrop.sock";
    static constexpr int POLL_INTERVAL_MS = 500;
};