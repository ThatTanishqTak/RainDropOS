#!/usr/bin/env bash
set -e

# Determine root directory (one level up from script location)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

###### 1. Build host-side components using CMake (converted from Windows .bat) ######
CMAKE_BUILD_DIR="$ROOT_DIR/build"

mkdir -p "$CMAKE_BUILD_DIR"
pushd "$CMAKE_BUILD_DIR" > /dev/null
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
popd > /dev/null

###### 2. Build RainDropOS kernel and ISO (existing build.sh logic) ######
OS_BUILD_DIR="$ROOT_DIR/os_build"
ISO_DIR="$ROOT_DIR/iso"

rm -rf "$OS_BUILD_DIR" "$ISO_DIR" RainDropOS.iso
mkdir -p "$OS_BUILD_DIR" "$ISO_DIR/boot/grub"

# Assemble bootstrap
nasm -f elf32 "$ROOT_DIR/asm/bootstrap.asm" -o "$OS_BUILD_DIR/bootstrap.o"

# Compile C++ kernel sources
CPPFLAGS="-ffreestanding -fno-exceptions -fno-rtti -m32 -std=c++17 -nostdlib -I $ROOT_DIR/include"
for src in $ROOT_DIR/src/*.cpp $ROOT_DIR/src/*/*.cpp; do
    obj="$OS_BUILD_DIR/$(basename "${src%.cpp}.o")"
    g++ $CPPFLAGS -c "$src" -o "$obj"
done

# Link kernel
ld -m elf_i386 -T "$ROOT_DIR/linker.ld" -o "$OS_BUILD_DIR/kernel.bin" "$OS_BUILD_DIR"/*.o

# Generate GRUB config
cat > "$ISO_DIR/boot/grub/grub.cfg" <<'CFG'
set timeout=0
set default=0

menuentry "RainDropOS" {
    multiboot /boot/kernel.bin
    boot
}
CFG

# Copy kernel and create ISO
cp "$OS_BUILD_DIR/kernel.bin" "$ISO_DIR/boot/kernel.bin"
grub-mkrescue -o RainDropOS.iso "$ISO_DIR" > /dev/null 2>&1

echo
echo "✨ Build complete — ISO image created: RainDropOS.iso"