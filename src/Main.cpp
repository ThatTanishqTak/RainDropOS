#include "RainDropOS.h"
#include "Kernel.h"
#include "Userland.h"
#include "Console.h"

extern "C" void RainDropOS_Boot();

namespace RainDropOS
{
    void Boot()
    {
        Console::Print("RainDropOS booting...\n");
        Kernel::Init();
        Userland::Init();
    }
}

extern "C" void RainDropOS_Boot()
{
    RainDropOS::Boot();
}