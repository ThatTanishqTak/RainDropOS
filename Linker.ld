ENTRY(RainDropOS_Bootstrap)

SECTIONS
{
    . = 1M;

    .text : { *(.text*) }
    .bss : { *(.bss*) }
}