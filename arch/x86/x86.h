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
