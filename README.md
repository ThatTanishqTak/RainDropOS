# RainDropOS

**RainDropOS** is a bespoke Linux environment crafted for gaming alone. The project draws inspiration from SteamOS while maintaining a minimal footprint.

## Building the ISO

Run `BuildISO.bat` to invoke the Makefile, assemble the bootloader, build the kernel with C++17, and produce `RainDropOS.bin`. The optional `make iso` step packages `RainDropOS.iso` using GRUB. Ensure `nasm`, `i686-elf-g++`, and `grub-mkrescue` are available on your system.

## License
RainDropOS is released under the MIT License. See [LICENSE](LICENSE) for details.