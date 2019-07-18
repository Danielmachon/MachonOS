#include "x86.h"

/* Initialize x86 arch */
void x86_init()
{
	gdt_init();
	idt_init();
	pic_init();
}