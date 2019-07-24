#include <stdint.h>
#include <stddef.h>
#include "../../term.h"
#include "x86.h"


/*
 * This file provides functions for IDT initialization and,
 * registration.
 *
 * The IDT (Interrupt Descriptor Table) is an,
 * array of mappings between interrupts and interrupt handlers.
 */

#define IDT_MAX_ENTRIES 255

/*
 * idt_entry struct
 * @base_low : Lower 16 bits of the addr to jump to
 * @selector : Kernel segment selector
 * @zero     : Must be zero
 * @flsgs    : Flags
 * @base_high: Upper 16 bis of the addr to jump to
 */
struct idt_entry
{
	uint16_t base_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t	flags;
	uint16_t base_high;
} __attribute__((packed));

/*
 * idt_ptr struct
 * @limit: N/A
 * base  : Addr of the 1st element of struct idt_entry
 */
struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct idt_ptr idt;

/* kernel.c */
struct idt_entry idt_addr[IDT_MAX_ENTRIES];

/*
 * Register a single new interrupt with the,
 * interrupt descriptor table.
 *
 * @cb:  function callback to register.
 * @irq: interrupt request.
 */
void idt_register_irq(uint8_t irq, uint32_t cb)
{
	//terminal_writestring("Reg\n");

	struct idt_entry *ent = &idt_addr[irq];

	ent->base_low = (uint16_t) (cb & 0x0000ffff);
	ent->selector = (uint16_t) 0x8;
	ent->zero = 0x00;
	ent->flags = 0x8e; //0 | IDT_32BIT_INTERRUPT_GATE | IDT_PRESENT;
	ent->base_high = (uint16_t) ((cb & 0xffff0000) >> 16);
}

/* Assembly function */
extern void _idt_load();

/*
 * idt_load
 *
 * Load the interrupt descriptor table into the CPU.
 *
 * @idt: address of struct idt_ptr.
 *
 */
void idt_init()
{
	idt.limit = (sizeof(struct idt_entry) * IDT_MAX_ENTRIES) - 1;
	idt.base = (uint32_t) &idt_addr;

	/* Register keyboard interrupt */
	int x;
	for (x = 1; x < 255; x++)
		idt_register_irq(x, (uint32_t) &_irq_handler);
	
	_idt_load(&idt);
}
