#include "Console.h"

namespace RainDropOS
{
    namespace Kernel
    {
        void init()
        {
            Console::Print("Kernel initialized.\n");
        }
    }
}