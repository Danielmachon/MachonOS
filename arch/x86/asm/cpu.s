//Assembly functions for low-level cpu initialization

//Initialize the IDT
.global _idt_load
.type _idt_load, @function
_idt_load:
	push %ebp
	movl %esp, %ebp
	lidt 0x10f0    //8(%esp)		//load idt
	movl %ebp, %esp
	pop %ebp
	ret

//Initialize the GDT
.global _gdt_load
.type _gdt_load, @function
_gdt_load:
	push %ebp
	movl %esp, %ebp
	lgdt 8(%esp)		//load gdt
	movl %ebp, %esp
	pop %ebp
	ret

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

//Enable interrupts
.global _enable_interrupts
.type _enable_interrupts, @function
_enable_interrupts:
	push %ebp
	movl %esp, %ebp
	//Body
	pop %ebp
	ret

.global _go_to_protected
.type _go_to_protected, @function
_go_to_protected:
	push %ebp
	push %eax
	movl %esp, %ebp
	movl %cr0, %eax
	or 1, %eax
	movl %eax, %cr0
	pop %ebp
	pop %eax
	ret

// Handle interrupts
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