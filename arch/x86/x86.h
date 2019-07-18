#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* idt.c */
extern void idt_init();

/* gdt.c */
extern void gdt_init();

/* Initialize x86 arch */
void x86_init()
{
	gdt_init();
	idt_init();
}

/* io.s */
extern void _io_port_write_byte(uint16_t, uint8_t);
#define outb(port, value) _io_port_write_byte(port, value);

/* io.s */
extern uint8_t _io_port_read_byte(uint16_t);
#define inb(port) _io_port_read_byte(port);

/* io.s */
extern void _io_port_write_word(uint16_t, uint16_t);
#define outw(port, value) _io_port_write_word(port, value);

/* io.s */
extern uint16_t _io_port_read_word(uint16_t);
#define inw(port) _io_port_read_word(port);

/* io.s */
extern void _io_port_write_long(uint16_t, uint32_t);
#define outl(port, value) _io_port_write_long(port, value);

/* io.s */
extern uint32_t _io_port_read_long(uint16_t);
#define inl(port) _io_port_read_long(port);
