#include <stdint.h>

/* The Global Descriptor Table (GDT).

   The GDT, an x86-specific structure, defines segments that can
   potentially be used by all processes in a system, subject to
   their permissions.  There is also a per-process Local
   Descriptor Table (LDT) but that is not used by modern
   operating systems.

   Each entry in the GDT, which is known by its byte offset in
   the table, identifies a segment.  For our purposes only three
   types of segments are of interest: code, data, and TSS or
   Task-State Segment descriptors.  The former two types are
   exactly what they sound like.  The TSS is used primarily for
   stack switching on interrupts.

   For more information on the GDT as used here, refer to
   [IA32-v3a] 3.2 "Using Segments" through 3.5 "System Descriptor
   Types". */

/*
 * gdt_entry struct
 * @limit_low  :
 * @base_low   :
 * @base_middle:
 * @access     :
 * @granularity:
 * @base_high  :
 */
struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t	access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

/*
 * gdt_ptr struct
 * @limit: N/A
 * base  : Addr of the 1st element of struct gdt_entry
 */
struct gdt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));


struct gdt_entry gdt_addr[3];
struct gdt_ptr gdt_ptr;

/*
 * idt_register
 *
 * Set a single gdt entry in the gdt struct
 *
 * @num:    index in array
 * @base:
 * @limit:
 * @access:
 * @gran:
 */
static void gdt_set(uint32_t num, uint32_t base, uint32_t limit, uint32_t access, uint32_t gran)
{
	gdt_addr[num].base_low = (base & 0xFFFF);
        gdt_addr[num].base_middle = (base >> 16) & 0xFF;
        gdt_addr[num].base_high = (base >> 24) & 0xFF;
	gdt_addr[num].limit_low = (limit & 0xFFFF);
        gdt_addr[num].granularity = (limit >> 16) & 0x0F;
	gdt_addr[num].granularity |= (gran & 0xF0);
	gdt_addr[num].access = access;
}

extern void _gdt_load();
extern void _reload_segments();
extern void _go_to_protected();

/*
 * gdt_init
 *
 * @gdt: address of struct gdt_ptr.
 *
 */
void gdt_init()
{
	gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gdt_ptr.base = (uint32_t) &gdt_addr;

	/* Set segments */
	gdt_set(0, 0, 0, 0, 0);
	gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	asm volatile("cli");

	/* Load the GDT */
	_gdt_load(gdt_ptr);

	/* Reload segments */
	_reload_segments();

	/* Go to protected mode */
//	_go_to_protected();
}
