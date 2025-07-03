; asm/bootstrap.asm
bits 16
org 0x7C00
global _start

_start:
    cli                 ; disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; stack grows down from the bootloader location

    ; Here we could load further sectors or jump directly to C++ code
    call Boot           ; Boot is our C++17 entry function
    jmp $               ; hang after Boot returns

times 510 - ($-$$) db 0 ; pad out to 512 bytes
dw 0xAA55              ; boot signature