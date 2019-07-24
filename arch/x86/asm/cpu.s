//Assembly functions for low-level cpu initialization

// Initialize the IDT
.global _idt_load
.type _idt_load, @function
_idt_load:
	pushl %ebp
	movl %esp, %ebp
	pushl %edx
	movl 8(%ebp), %edx
	lidt (%edx)		//load idt
	popl %edx
	movl %ebp, %esp
	popl %ebp
	ret

// Initialize the GDT
.global _gdt_load
.type _gdt_load, @function
_gdt_load:
	pushl %ebp
	movl %esp, %ebp
	pushl %edx
	movl 8(%ebp), %edx
	lgdt (%edx)		//load gdt
	popl %edx
	movl %ebp, %esp
	popl %ebp
	ret

// Reload segments
.global _reload_segments
.type _reload_segments, @function
_reload_segments:
	push %ebp
	movl %esp, %ebp
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	pop %eax
	ljmp $0x08, $flush 	//far jump to 0x08 and place $flush in EIP
flush:
	movl %ebp, %esp
	pop %ebp
	ret

// Handle IRQ's
.global irq_handler
.global _irq_handler
.align 4
.type _irq_handler, @function
_irq_handler:
	push %ebp
	movl %esp, %ebp
	call irq_handler
	movl %ebp, %esp
	pop %ebp
	iretl