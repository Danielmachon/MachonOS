#include <stdint.h>


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
};

/*
 * idt_ptr struct
 * @limit: N/A
 * base  : Addr of the 1st element of struct idt_entry
 */
struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
};

/* kernel.c */
struct idt_entry *idt_addr;

/*
 * Register a single new interrupt with the,
 * interrupt descriptor table.
 *
 * @cb:  function callback to register.
 * @irq: interrupt request.
 */
void idt_register_irq(uint8_t irq, uint32_t cb)
{
	*(uint16_t*)(idt_addr + 8 * irq + 0) = (uint16_t)(cb & 0x0000ffff);
	*(uint16_t*)(idt_addr + 8 * irq + 2) = (uint16_t)0x8;
	*(uint8_t*) (idt_addr + 8 * irq + 4) = 0x00;
	*(uint8_t*) (idt_addr + 8 * irq + 5) = 0x8e;//0 | IDT_32BIT_INTERRUPT_GATE | IDT_PRESENT;
	*(uint16_t*)(idt_addr + 8 * irq + 6) = (uint16_t)((cb & 0xffff0000) >> 16);
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
void idt_init(uint32_t *idt)
{
	_idt_load(idt);
}
