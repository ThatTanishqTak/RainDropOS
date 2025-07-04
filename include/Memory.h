#pragma once
#include <stddef.h>

namespace RainDropOS
{
    namespace Kernel
    {
        namespace Memory
        {
            void Init();
            void* Allocate(size_t size);
            void Reset();
        }
    }
}