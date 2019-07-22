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
struct idt_entry idt_addr[256];

static uint32_t idt_loc = 0x2000;
static uint32_t idtr_loc = 0x10f0;
static uint16_t idt_size = 0x800;

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

/*
 * Register a single new interrupt with the,
 * interrupt descriptor table.
 *
 * @cb:  function callback to register.
 * @irq: interrupt request.
 */
 
void idt_register_irq_raw(uint8_t irq, uint32_t cb)
{
	*(uint16_t*)(idt_loc + 8*irq + 0) = (uint16_t)(cb & 0x0000ffff);
	*(uint16_t*)(idt_loc + 8*irq + 2) = (uint16_t)0x8;
	*(uint8_t*) (idt_loc + 8*irq + 4) = 0x00;
	*(uint8_t*) (idt_loc + 8*irq + 5) = 0x8e;//0 | IDT_32BIT_INTERRUPT_GATE | IDT_PRESENT;
	*(uint16_t*)(idt_loc + 8*irq + 6) = (uint16_t)((cb & 0xffff0000) >> 16);
}

uint8_t inportb(uint16_t portid)
{
	uint8_t ret;
	asm volatile("inb %%dx, %%al":"=a"(ret):"d"(portid));
	return ret;
}

void outportb(uint16_t portid, uint8_t value)
{
	asm volatile("outb %%al, %%dx": :"d" (portid), "a" (value));
}

/*
 * idt_entry_new
 *
 * Register a single new interrupt with the,
 * interrupt descriptor table.
 *
 * @irq:       interrupt request.
 * @new_entry: New entry to be registered.
 * @ret: struct idt_entry returned.

static const struct idt_entry* idt_entry_new(uint8_t irq, const struct idt_entry *new_entry)
{
	static uint8_t idx;
	struct idt_entry *entry;

	entry = (idt_addr + idx);
	entry->base_low = new_entry->base_low;
	entry->selector = new_entry->selector;
	entry->zero = new_entry->zero;
	entry->flags = new_entry->flags;
	entry->base_high = new_entry->base_high;
}
*/

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
	//idt.limit = (sizeof(struct idt_entry) * 256) - 1;
	//idt.base = (uint32_t) &idt_addr[0];

	/* Register keyboard interrupt */
	int x;
	for (x = 1; x < 255; x++)
		idt_register_irq_raw(x, (uint32_t) &_irq_handler);

	*(uint16_t*)idtr_loc = idt_size - 1;
	*(uint32_t*)(idtr_loc + 2) = idt_loc;
	
	_idt_load((uint32_t) &idt);
}
