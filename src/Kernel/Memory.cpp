#include "Memory.h"
#include "Console.h"

namespace RainDropOS
{
    namespace Kernel
    {
        namespace Memory
        {
            namespace
            {
                constexpr size_t PoolSize = 64 * 1024;
                alignas(unsigned long long) unsigned char Pool[PoolSize];
                size_t Offset = 0;
            }

            void Init()
            {
                Offset = 0;
                Console::Print("Memory manager initialized.\n");
            }

            void* Allocate(size_t size)
            {
                if (Offset + size > PoolSize)
                {
                    return nullptr;
                }

                void* ptr = Pool + Offset;
                Offset += size;
                return ptr;
            }

            void Reset()
            {
                Offset = 0;
            }
        }
    }
}