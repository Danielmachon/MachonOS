#include <stdint.h>
#include <stddef.h>
#include "x86.h"
#include "../../term.h"

#define PIC1            0x20		    /* IO base address for master PIC */
#define PIC2            0xA0		    /* IO base address for slave PIC */

#define PIC1_COMMAND    PIC1            /* Master command port */
#define PIC1_DATA       (PIC1+1)        /* Master data port */
#define PIC2_COMMAND    PIC2            /* Slave command port */
#define PIC2_DATA       (PIC2+1)        /* Slave data port */
#define PIC_EOI         0x20            /* End of interrupt */

#define ICW1_ICW4	    0x01            /* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		    /* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		    /* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		    /* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		    /* Initialization - required! */
 
#define ICW4_8086	    0x01		    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08    		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	    	/* Buffered mode/master */
#define ICW4_SFNM	    0x10		    /* Special fully nested (not) */

/* 
IRQ 0 ‒ system timer
IRQ 1 — keyboard controller
IRQ 3 — serial port COM2
IRQ 4 — serial port COM1
IRQ 5 — line print terminal 2
IRQ 6 — floppy controller
IRQ 7 — line print terminal 1
IRQ 8 — RTC timer
IRQ 12 — mouse controller
IRQ 13 — math co-processor
IRQ 14 — ATA channel 1
IRQ 15 — ATA channel 2 
*/

/*
 * Send end-of-interrupt to pic.
 *
 * @irq: interrupt routine number.
 *
 * If the interrupt was send from the slave, we have to send an,
 * eoi to both the master and the slave, otherwise the master only.
 */
void pic_eoi(uint8_t irq)
{
    if(irq >= 8)
        outportb(PIC2_COMMAND, PIC_EOI);

	outportb(PIC1_COMMAND, PIC_EOI);
}

/*
 * Remap the PIC.
 *
 * The pic expects three words after the initialization bye (0x11) has been received:
 *     Its vector offset. (ICW2)
 *     Tell it how it is wired to master/slaves. (ICW3)
 *     Gives additional information about the environment. (ICW4)
 *
 * It is necessary to wait a small amount of time, to let the pic,
 * process the word, on some older platforms - hence the use of,
 * outb_wait (call to outb followed by io_wait).
 */
static void pic_remap()
{
    terminal_writestring("Remapping pic\n");

    /* Save current masks */
    uint8_t m1, m2;

    m1 = inportb(PIC1_DATA);
    m2 = inportb(PIC2_DATA);

    /* Initialize the remapping */
	outportb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outportb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Send words */
	outportb(PIC1_DATA, PIC1);                    // ICW2: Master PIC vector offset
	outportb(PIC2_DATA, PIC2);                    // ICW2: Slave PIC vector offset
	outportb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outportb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outportb(PIC1_DATA, ICW4_8086);               // ICW4:
	outportb(PIC2_DATA, ICW4_8086);               // ICW4:  


    /* Restore masks */
    outportb(PIC1_DATA, 0); 
	outportb(PIC2_DATA, 0);
}

/*
 * Mask specific interrupt.
 *
 * @irq: irq to be masked.
 */
void pic_mask(uint8_t irq)
{
    outportb(0x21, 0xfd);
    outportb(0xa1, 0xff);
    return;

    uint16_t port;
    uint8_t imr;

    if (irq <= 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    /* Read the IMR register */
    imr = inportb(port);
    imr |= (1 << irq);

    /* Write the irq to be masked */
    outportb(port, imr);
}

void pic_init()
{
    pic_remap();
}
