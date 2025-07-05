[bits 32]

SECTION .multiboot
    dd 0x1BADB002
    dd 0
    dd -(0x1BADB002)

SECTION .text
    global RainDropOS_Bootstrap
RainDropOS_Bootstrap:
    mov esp, stack_top
    extern RainDropOS_Boot
    call RainDropOS_Boot
.hang:
    jmp .hang

SECTION .bss
    resb 4096
stack_top: