/* Assembly functions for low-level io */

// Write a single byte to io port
.global _io_port_write_byte
.type _io_port_write_byte, @function
_io_port_write_byte:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %eax      // port id
    movl 12(%esp), %ebx     // value
	outb %ax, %bl           // write value to port
	movl %ebp, %esp
    pop %ebx, %eax
	pop %ebp
	ret

// Read a single byte from io port
.global _io_port_read_byte
.type _io_port_read_byte, @function
_io_port_read_byte:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %ebx      // port id
	inb %bx, %al            // read value and store in return register
	movl %ebp, %esp
    pop %ebx, %eax
    pop %ebp
	ret

// Write a single word to io port
.global _io_port_write_word
.type _io_port_write_word, @function
_io_port_write_word:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %eax      // port id
    movl 12(%esp), %ebx     // value
	outw %ax, %bx           // write value to port
	movl %ebp, %esp
    pop %ebx, %eax
	pop %ebp
	ret

// Read a single word from io port
.global _io_port_read_word
.type _io_port_read_word, @function
_io_port_read_word:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %ebx      // port id
	inw %bx, %ax            // read value and store in return register
	movl %ebp, %esp
    pop %ebx, %eax
    pop %ebp
	ret

// Write a single long to io port
.global _io_port_write_long
.type _io_port_write_long, @function
_io_port_write_long:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %eax      // port id
    movl 12(%esp), %ebx     // value
	outl %eax, %ebx         // write value to port
	movl %ebp, %esp
    pop %ebx, %eax
	pop %ebp
	ret

    // Read a single word from io port
.global _io_port_read_long
.type _io_port_read_long, @function
_io_port_read_long:
	push %ebp
    push %eax, %ebx
	movl %esp, %ebp
    movl 8(%esp), %ebx      // port id
	inl %ebx, %eax          // read value and store in return register
	movl %ebp, %esp
    pop %ebx, %eax
    pop %ebp
	ret