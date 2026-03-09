#include "Daemon.h"

#include <iostream>
#include <thread>
#include <chrono>

bool Daemon::Initialize()
{
    m_Server.SetHandler([this](const IpcMessage& msg)
    {
        return HandleMessage(msg);
    });

    if (!m_Server.Start(SOCKET_PATH))
    {
        std::cerr << "Daemon: failed to start IPC server\n";
        
        return false;
    }

    // Seed the CPU telemetry counters before the loop starts
    m_Telemetry.Update();

    m_Running = true;
    std::cout << "Daemon: initialized\n";

    return true;
}

void Daemon::Run()
{
    std::cout << "Daemon: running\n";

    while (m_Running)
    {
        m_Server.Poll();
        m_Telemetry.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(POLL_INTERVAL_MS));
    }
}

void Daemon::Stop()
{
    std::cout << "Daemon: stopping\n";
    m_Running = false;
    m_Server.Stop();
}

std::string Daemon::HandleMessage(const IpcMessage& message)
{
    switch (message.type)
    {
        case MessageType::GameStart:
            std::cout << "Daemon: game started — appid " << message.appid << "\n";
            m_Profiles.Apply(message.appid);
            return "";

        case MessageType::GameStop:
            std::cout << "Daemon: game stopped — appid " << message.appid << "\n";
            m_Profiles.RestoreDefaults();
            return "";

        case MessageType::GetTelemetry:
            return m_Telemetry.GetLatest().Serialize();

        default:
            std::cerr << "Daemon: unknown message type\n";
            return "";
    }
}