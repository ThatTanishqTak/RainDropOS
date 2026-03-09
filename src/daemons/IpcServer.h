#pragma once

#include <string>
#include <functional>
#include "../../include/IpcMessages.h"

class IpcServer
{
public:
    using MessageHandler = std::function<std::string(const IpcMessage&)>;

    bool Start(const std::string& socketPath);
    void Stop();
    void Poll();

    void SetHandler(MessageHandler handler) { m_Handler = handler; }
    bool IsRunning() const { return m_ServerFd >= 0; }

private:
    int m_ServerFd = -1;
    std::string m_SocketPath;
    MessageHandler m_Handler;
};