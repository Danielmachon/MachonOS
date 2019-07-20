#include "x86.h"

/* Initialize x86 arch */
void x86_init()
{
	asm volatile ("cli");

	gdt_init();
	idt_init();
	pic_init();

	/* Register keyboard interrupt */
	idt_register_irq(1, (uint32_t) _irq_handler);

	asm volatile ("sti");
}