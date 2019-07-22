#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void x86_init();
void irq_handler();

/* idt.c */
extern void idt_init();
extern void idt_register_irq(uint8_t, uint32_t);
extern void idt_register_irq_raw(uint8_t, uint32_t);

/* gdt.c */
extern void gdt_init();

/* pic.c */
extern void pic_init();
extern void pic_mask(uint8_t);

/* cpu.s */
extern void _irq_handler();
extern void _dummy();

/* io.s */
extern void _io_port_write_byte(uint16_t, uint8_t);
#define outb(port, value) _io_port_write_byte(port, value);

#define outb_wait(port, value) do { \
	_io_port_write_byte(port, value); \
	_io_wait(); \
} while(0)

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

/* io.s */
extern void _io_wait();
#define io_wait() _io_wait();

extern uint8_t inportb(uint16_t portid);
extern void outportb(uint16_t portid, uint8_t value);
