#include "IpcServer.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

bool IpcServer::Start(const std::string& socketPath)
{
    m_SocketPath = socketPath;

    // Remove stale socket file if it exists
    unlink(socketPath.c_str());

    m_ServerFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_ServerFd < 0)
    {
        std::cerr << "IpcServer: socket() failed\n";
        
        return false;
    }

    // Non-blocking so Poll() doesn't hang
    fcntl(m_ServerFd, F_SETFL, O_NONBLOCK);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);

    if (bind(m_ServerFd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "IpcServer: bind() failed\n";
        close(m_ServerFd);
        m_ServerFd = -1;

        return false;
    }

    if (listen(m_ServerFd, 5) < 0)
    {
        std::cerr << "IpcServer: listen() failed\n";
        close(m_ServerFd);
        m_ServerFd = -1;
    
        return false;
    }

    std::cout << "IpcServer: listening on " << socketPath << "\n";

    return true;
}

void IpcServer::Stop()
{
    if (m_ServerFd >= 0)
    {
        close(m_ServerFd);
        m_ServerFd = -1;
    }

    unlink(m_SocketPath.c_str());
    std::cout << "IpcServer: stopped\n";
}

void IpcServer::Poll()
{
    if (m_ServerFd < 0)
    {
        return;
    }

    // Accept a connection — non-blocking, returns immediately if none
    int clientFd = accept(m_ServerFd, nullptr, nullptr);
    if (clientFd < 0)
    {
        return;
    }

    // Read the message
    char buffer[4096] = {};
    ssize_t n = read(clientFd, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        std::string raw(buffer, n);
        IpcMessage msg = IpcMessage::Deserialize(raw);

        // Call handler and send response if there is one
        if (m_Handler)
        {
            std::string response = m_Handler(msg);
            if (!response.empty())
            {
                write(clientFd, response.c_str(), response.size());
            }
        }
    }

    close(clientFd);
}