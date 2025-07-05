CROSS ?= i686-elf-
AS=nasm
CC=$(CROSS)g++
LD=$(CROSS)g++

CXXFLAGS=-std=c++17 -ffreestanding -c
LDFLAGS=-T linker.ld -ffreestanding -nostdlib
ASFLAGS=-f elf32

OBJDIR=build
ISO_DIR=iso
ASM_SRC=asm/bootstrap.asm
CPP_SRC=src/Main.cpp
OBJS=$(OBJDIR)/bootstrap.o $(OBJDIR)/Main.o
BIN=RainDropOS.bin
ISO=RainDropOS.iso

all: $(BIN)

$(OBJDIR)/bootstrap.o: $(ASM_SRC)
	mkdir -p $(OBJDIR)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/Main.o: $(CPP_SRC)
	mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) $< -o $@

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

iso: $(BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN) $(ISO_DIR)/boot/$(BIN)
	echo 'set timeout=0\nmenuentry "RainDropOS" { multiboot /boot/$(BIN) }' > $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

clean:
	rm -f $(OBJS) $(BIN) $(ISO)
	rm -rf $(ISO_DIR)
.PHONY: all clean iso