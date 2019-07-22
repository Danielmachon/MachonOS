#include <stdint.h>
#include <stddef.h>
#include "../../term.h"
#include "x86.h"

void irq_handler()
{
    terminal_writestring("Caught interrupt!\n");

    pic_eoi(1);

    uint8_t b = inportb(0x64);
    uint8_t c = inportb(0x60);
}