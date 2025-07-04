#include "Console.h"

namespace RainDropOS
{
    namespace Kernel
    {
        void Init()
        {
            Console::Print("Kernel initialized.\n");
        }
    }
}