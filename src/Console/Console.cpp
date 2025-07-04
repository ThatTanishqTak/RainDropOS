#include "Console.h"
#include <stddef.h>
#include <stdint.h>

namespace RainDropOS
{
    namespace Console
    {
        constexpr uintptr_t FrameBufferAddress = 0xB8000;

        class FrameBuffer
        {
        public:
            volatile unsigned short* const Data = reinterpret_cast<unsigned short*>(FrameBufferAddress);
            unsigned int Cursor = 0;

            inline void PutChar(char c)
            {
                Data[Cursor++] = (0x07 << 8) | static_cast<unsigned char>(c);
            }
        };

        static FrameBuffer FrameBuf;

        void Print(const char* str)
        {
            while (*str)
            {
                FrameBuf.PutChar(*str++);
            }
        }
    }
}