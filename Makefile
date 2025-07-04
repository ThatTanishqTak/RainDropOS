	# RainDropOS Makefile

OS_BUILD_DIR := os_build
ISO_DIR := iso

CPPFLAGS := -ffreestanding -fno-exceptions -fno-rtti -m32 -std=c++17 -nostdlib -I include -Wall -Wextra -Wpedantic
ASFLAGS := -f elf32
LDFLAGS := -m elf_i386 -T linker.ld

CPP_SOURCES := $(wildcard src/*.cpp src/*/*.cpp)
CPP_OBJECTS := $(patsubst src/%.cpp,$(OS_BUILD_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(OS_BUILD_DIR)/bootstrap.o $(CPP_OBJECTS)

all: kernel

$(OS_BUILD_DIR):
	mkdir -p $@

$(OS_BUILD_DIR)/bootstrap.o: asm/bootstrap.asm | $(OS_BUILD_DIR)
	nasm $(ASFLAGS) $< -o $@

$(OS_BUILD_DIR)/%.o: src/%.cpp | $(OS_BUILD_DIR)
	mkdir -p $(dir $@)
	g++ $(CPPFLAGS) -c $< -o $@

kernel: $(OBJECTS)
	ld $(LDFLAGS) -o $(OS_BUILD_DIR)/kernel.bin $(OBJECTS)

iso: kernel
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OS_BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	@echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'menuentry "RainDropOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o RainDropOS.iso $(ISO_DIR) > /dev/null 2>&1
	@echo
	@echo "\u2728 Build complete — ISO image created: RainDropOS.iso"

clean:
	rm -rf $(OS_BUILD_DIR) $(ISO_DIR) RainDropOS.iso

test: $(OS_BUILD_DIR)/tests
	$(OS_BUILD_DIR)/tests

$(OS_BUILD_DIR)/tests: test/TestMain.cpp | $(OS_BUILD_DIR)
	g++ -std=c++17 -Wall -Wextra -Wpedantic -I include $< -o $@

.PHONY: all kernel iso clean test