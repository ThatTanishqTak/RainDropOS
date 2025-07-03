#include "Console.h"

namespace RainDropOS
{
    namespace Console
    {
        static volatile unsigned short* video = (unsigned short*)0xB8000;
        static int cursor = 0;

        void Print(const char* str)
        {
            while (*str)
            {
                video[cursor++] = (0x07 << 8) | *str++;
            }
        }
    }
}