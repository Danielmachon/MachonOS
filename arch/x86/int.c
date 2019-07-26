#include <stdint.h>
#include <stddef.h>
#include "../../kernel/term.h"
#include "x86.h"

void irq_handler()
{
    terminal_writestring("Caught interrupt!\n");

    pic_eoi(1);

    uint8_t b = inb(0x64);
    uint8_t c = inb(0x60);
}
