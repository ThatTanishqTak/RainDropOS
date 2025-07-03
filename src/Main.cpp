#include "RainDropOS.h"

#include <iostream>

namespace RainDropOS
{
    void Boot()
    {
        std::cout << "RainDropOS booting..." << std::endl;
    }
}

int main()
{
    RainDropOS::Boot();

    return 0;
}