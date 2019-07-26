MAKE_DIR = $(PWD)/osdev/MachonOS
ISODIR = $(MAKE_DIR)/isodir
BINDIR = $(MAKE_DIR)/isodir/boot

AS = /usr/local/cross/bin/i686-elf-as
CC = /usr/local/cross/bin/i686-elf-gcc
CFLAGS =-std=gnu99 -ffreestanding -Wall -Wextra

ARCH_SRC_DIR := $(MAKE_DIR)/arch/x86
ARCH_ASM_DIR := $(MAKE_DIR)/arch/x86/asm

INC=

all: os
	
test: printf
	
printf-clean: printf-clean

# top

kernel.o: $(MAKE_DIR)/kernel/kernel.c
	$(CC) -c $(CFLAGS) $(MAKE_DIR)/kernel/kernel.c $(INC)

term.o: $(MAKE_DIR)/kernel/term.c
	$(CC) -c $(CFLAGS) $(MAKE_DIR)/kernel/term.c $(INC)

# arch

gdt.o: $(ARCH_SRC_DIR)/gdt.c
	$(CC) -c $(CFLAGS) $(ARCH_SRC_DIR)/gdt.c $(INC)

idt.o: $(ARCH_SRC_DIR)/idt.c
	$(CC) -c $(CFLAGS) $(ARCH_SRC_DIR)/idt.c $(INC)
	
int.o: $(ARCH_SRC_DIR)/int.c
	$(CC) -c $(CFLAGS) $(ARCH_SRC_DIR)/int.c $(INC)

pic.o: $(ARCH_SRC_DIR)/pic.c
	$(CC) -c $(CFLAGS) $(ARCH_SRC_DIR)/pic.c $(INC)

x86.o: $(ARCH_SRC_DIR)/x86.c
	$(CC) -c $(CFLAGS) $(ARCH_SRC_DIR)/x86.c $(INC)

# asm

boot.o: $(ARCH_ASM_DIR)/boot.s
	$(AS) arch/x86/asm/boot.s -o boot.o

cpu.o: $(ARCH_ASM_DIR)/cpu.s
	$(AS) arch/x86/asm/cpu.s -o cpu.o

io.o: $(ARCH_ASM_DIR)/io.s
	$(AS) arch/x86/asm/io.s -o io.o

os: iso

iso: boot.o int.o io.o idt.o gdt.o pic.o cpu.o kernel.o term.o x86.o
	$(CC) -T linker.ld -o MachonOS.bin -ffreestanding -nostdlib boot.o int.o io.o idt.o gdt.o pic.o cpu.o kernel.o term.o x86.o -lgcc
	cp MachonOS.bin $(BINDIR)
	grub-mkrescue -o MachonOS.iso isodir

clean: 
	rm *.o
	
run:
	qemu-system-i386 -kernel $(BINDIR)/MachonOS.bin

printf: $(MAKE_DIR)/kernel/printf.c
	   $(CC) $(MAKE_DIR)/kernel/printf.c -o printf -ffreestanding -nostdlib -g
	
printf-clean:
	rm $(MAKE_DIR)/kernel/printf