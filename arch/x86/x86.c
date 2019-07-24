#include <stddef.h>
#include "x86.h"
#include "../../term.h"

/* Initialize x86 arch */
void x86_init()
{
	asm volatile ("cli");

	gdt_init();
	idt_init();
	pic_init();
	pic_mask(0x00);

	asm volatile ("sti");

	terminal_writestring("Done initializing arch\n");
}