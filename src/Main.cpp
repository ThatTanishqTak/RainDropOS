#include "RainDropOS.h"
#include "Kernal.h"
#include "Userland.h"
#include "Console.h"

extern "C" void RainDropOS_Boot();

namespace RainDropOS
{
    void Boot()
    {
        Console::Print("RainDropOS booting...\n");
        Kernel::init();
        Userland::init();
    }
}

extern "C" void RainDropOS_Boot()
{
    RainDropOS::Boot();
}