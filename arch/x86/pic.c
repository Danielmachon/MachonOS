#include <stdint.h>
#include "x86.h"

#define PIC1            0x20		    /* IO base address for master PIC */
#define PIC2            0xA0		    /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
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
		outb(PIC2_COMMAND, PIC_EOI);

	outb(PIC1_COMMAND, PIC_EOI);
}

/*
 * Remap the PIC.
 *
 * The pic expects three words after the initialization bye (0x11) has been received:
 *     Its vector offset. (ICW2)
 *     Tell it how it is wired to master/slaves. (ICW3)
 *     Gives additional information about the environment. (ICW4)
 */
static void pic_remap()
{
    /* Save current masks */
    uint8_t m1, m2;

    m1 = inb(PIC1_DATA);
    //m2 = inb(PIC2_DATA);

    /* Initialize the remapping */
	//outb_wait(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//outb_wait(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Send words */
	//outb_wait(PIC1_DATA, PIC1);                    // ICW2: Master PIC vector offset
	//outb_wait(PIC2_DATA, PIC2);                    // ICW2: Slave PIC vector offset
	//outb_wait(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	//outb_wait(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    //outb_wait(PIC1_DATA, ICW4_8086);               // ICW4:
	//outb_wait(PIC2_DATA, ICW4_8086);               // ICW4:  


    /* Restore masks */
    //outb_wait(PIC1_DATA, m1); 
	//outb_wait(PIC2_DATA, m2);
}

void pic_init()
{
    pic_remap();
}
