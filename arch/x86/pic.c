#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

void pic_eoi()
{

}

/*
 * Remap the PIC.

 * The pic expects three words after the initialization bye (0x11) has been received:
 *     Its vector offset. (ICW2)
 *     Tell it how it is wired to master/slaves. (ICW3)
 *     Gives additional information about the environment. (ICW4)
 */
void pic_remap()
{
    /* Save current masks */

    /* Initialize the remapping */

    /* Send words */

    /* Restore masks  */
}