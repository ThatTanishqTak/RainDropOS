#include <stdint.h>
#include <stddef.h>

extern "C" void RainDropOS_Boot()
{
    const char* msg = "RainDropOS";
    volatile uint16_t* video = reinterpret_cast<uint16_t*>(0xB8000);
    for (size_t i = 0; msg[i]; ++i)
        video[i] = (uint16_t)msg[i] | 0x0700;

    while (true) {}
}