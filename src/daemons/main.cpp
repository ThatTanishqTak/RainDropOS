#include "Daemon.h"

#include <iostream>
#include <csignal>

static Daemon* g_Daemon = nullptr;

static void OnSignal(int)
{
    std::cout << "\nDaemon: caught signal, shutting down\n";
    if (g_Daemon)
    {
        g_Daemon->Stop();
    }
}

int main()
{
    std::cout << "RainDrop Daemon starting\n";

    Daemon daemon;
    g_Daemon = &daemon;

    // Clean shutdown on Ctrl+C or kill
    std::signal(SIGINT,  OnSignal);
    std::signal(SIGTERM, OnSignal);

    if (!daemon.Initialize())
    {
        std::cerr << "Daemon: initialization failed\n";

        return 1;
    }

    daemon.Run();

    return 0;
}