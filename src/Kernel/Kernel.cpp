#include "Console.h"
#include "Memory.h"

namespace RainDropOS
{
    namespace Kernel
    {
        void Init()
        {
            Console::Print("Kernel initialized.\n");
            Memory::Init();

            if (Memory::Allocate(128))
            {
                Console::Print("Memory test passed.\n");
            }
        }
    }
}