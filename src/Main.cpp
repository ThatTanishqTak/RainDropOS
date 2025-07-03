#include "RainDropOS.h"
#include "Kernal.h"
#include "Userland.h"

#include <iostream>

namespace RainDropOS
{
    void Boot()
    {
        std::cout << "RainDropOS booting..." << std::endl;
        Kernel::init();
        Userland::init();
    }
}

int main()
{
    RainDropOS::Boot();
    std::cin.get();

    return 0;
}