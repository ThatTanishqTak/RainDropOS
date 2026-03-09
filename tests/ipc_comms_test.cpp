#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "../src/daemons/IpcServer.h"
#include "../src/shell/IpcClient.h"

int main()
{
    const std::string socketPath = "/tmp/raindrop_test.sock";

    // Start server in a background thread
    IpcServer server;
    std::string receivedAppid;

    server.SetHandler([&](const IpcMessage& msg) -> std::string
    {
        if (msg.type == MessageType::GameStart)
        {
            receivedAppid = msg.appid;
            std::cout << "Server received game_start for appid: " << msg.appid << "\n";
        }
        else if (msg.type == MessageType::GetTelemetry)
        {
            TelemetryData data;
            data.cpu_percent  = 55.0f;
            data.temp_celsius = 68.0f;
            data.ram_used_mb  = 3200.0f;

            return data.Serialize();
        }

        return "";
    });

    if (!server.Start(socketPath))
    {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    // Run server polling in background thread
    bool running = true;
    std::thread serverThread([&]()
    {
        while (running)
        {
            server.Poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // Give server a moment to start
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Test with client
    IpcClient client;

    // Override socket path by temporarily symlinking — 
    // instead just test directly against the server socket
    // by connecting manually for this test
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);

    // Test 1 — send game_start
    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) == 0)
    {
        IpcMessage msg;
        msg.type  = MessageType::GameStart;
        msg.appid = "440";
        std::string raw = msg.Serialize();
        write(fd, raw.c_str(), raw.size());
        close(fd);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Test 2 — request telemetry
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) == 0)
    {
        IpcMessage req;
        req.type = MessageType::GetTelemetry;
        std::string raw = req.Serialize();
        write(fd, raw.c_str(), raw.size());

        char buffer[4096] = {};
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);

        if (n > 0)
        {
            TelemetryData data = TelemetryData::Deserialize(std::string(buffer, n));
            std::cout << "CPU:  " << data.cpu_percent  << "%\n";
            std::cout << "Temp: " << data.temp_celsius << "C\n";
            std::cout << "RAM:  " << data.ram_used_mb  << "MB\n";
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Results
    std::cout << "Appid received: " << (receivedAppid == "440" ? "PASS" : "FAIL") << "\n";

    running = false;
    serverThread.join();
    server.Stop();

    return 0;
}