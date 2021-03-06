#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../arch/arch.h"	//Architecture specific functions
#include "term.h"		//Terminal output

void kernel_init()
{
	arch_init = &x86_init;
	arch_init();
}

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminal_init();

	/* Architecture specific initialization */
	kernel_init();

	terminal_writestring("Welcome to MachonOS!\n");
	terminal_writestring("There's not much to do here right now ..\n");

	for(;;) {
    	asm("hlt");
 	}
}
