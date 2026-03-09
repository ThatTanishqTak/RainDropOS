#include "IpcClient.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

bool IpcClient::IsAvailable() const
{
    return access(SOCKET_PATH, F_OK) == 0;
}

bool IpcClient::Send(const IpcMessage& message)
{
    if (!IsAvailable())
    {
        return false;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        return false;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(fd);

        return false;
    }

    std::string raw = message.Serialize();
    write(fd, raw.c_str(), raw.size());
    close(fd);

    return true;
}

TelemetryData IpcClient::RequestTelemetry()
{
    TelemetryData empty;
    if (!IsAvailable())
    {
        return empty;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        return empty;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(fd);
        return empty;
    }

    // Send the request
    IpcMessage req;
    req.type = MessageType::GetTelemetry;
    std::string raw = req.Serialize();
    write(fd, raw.c_str(), raw.size());

    // Read the response
    char buffer[4096] = {};
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (n > 0)
    {
        return TelemetryData::Deserialize(std::string(buffer, n));
    }

    return empty;
}