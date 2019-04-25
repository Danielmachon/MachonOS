CC = i686-elf-gcc

main: kernel.c term.c arch/x86/idt.c arch/x86/gdt.c linker.ld arch/x86/asm/boot.s arch/x86/asm/cpu.s
	i686-elf-as arch/x86/asm/boot.s -o boot.o
	i686-elf-as arch/x86/asm/cpu.s -o cpu.o
	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -Wall -Wextra
	$(CC) -c term.c -o term.o -std=gnu99 -ffreestanding -Wall -Wextra
	$(CC) -c arch/x86/idt.c -o idt.o -std=gnu99 -ffreestanding -Wall -Wextra
	$(CC) -c arch/x86/gdt.c -o gdt.o -std=gnu99 -ffreestanding -Wall -Wextra
	$(CC) -T linker.ld -o MachonOS.bin -ffreestanding -nostdlib boot.o idt.o gdt.o cpu.o kernel.o term.c -lgcc
	cp MachonOS.bin isodir/boot
	grub-mkrescue -o MachonOS.iso isodir

clean:
	rm ./*.o ./*.bin ./*.iso ./isodir/boot/*.bin
