#pragma once

#include "../../include/IpcMessages.h"

class IpcClient
{
public:
    bool IsAvailable() const;
    bool Send(const IpcMessage& message);

    TelemetryData RequestTelemetry();

private:
    static constexpr const char* SOCKET_PATH = "/tmp/raindrop.sock";
};