# RainDropOS

**RainDropOS** is a bespoke Linux environment crafted for gaming alone. The project draws inspiration from SteamOS while maintaining a minimal footprint.

## Building the ISO

Run `scripts/BuildISO.sh` to assemble the bootloader, build the kernel with C++17,
and create `RainDropOS.iso` using GRUB. The script requires `nasm`, `grub-pc-bin`
and `xorriso` installed on your system.

## License
RainDropOS is released under the MIT License. See [LICENSE](LICENSE) for details.