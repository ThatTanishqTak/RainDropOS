; Basic Multiboot-compliant bootstrap for RainDropOS
BITS 32

section .multiboot
    align 4
    dd 0x1BADB002          ; magic
    dd 0x00                ; flags
    dd -(0x1BADB002 + 0x00)

section .bss
    align 4
stack_bottom:
    resb 16384
stack_top:

section .text
    global start
    extern RainDropOS_Boot

start:
    mov esp, stack_top
    call RainDropOS_Boot
.hang:
    cli
    hlt
    jmp .hang