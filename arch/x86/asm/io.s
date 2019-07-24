/* Assembly functions for low-level io */

// Write a single byte to io port
.global _io_port_write_byte
.type _io_port_write_byte, @function
_io_port_write_byte:
	push %ebp
	movl %esp, %ebp
	push %eax
	push %edx
	movl 8(%ebp), %edx		// port id
	movl 12(%ebp), %eax		// value							
	outb %al, %dx           // write value in %al to port in %dx
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Read a single byte from io port
.global _io_port_read_byte
.type _io_port_read_byte, @function
_io_port_read_byte:
	push %ebp
	movl %esp, %ebp
    push %eax
	push %edx
	movl 8(%ebp), %edx      // port id
	inb %dx, %al            // read value and store in return register
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Write a single word to io port
.global _io_port_write_word
.type _io_port_write_word, @function
_io_port_write_word:
	push %ebp
	movl %esp, %ebp
	push %eax
	push %edx
	movl 8(%esp), %edx      // port id
	movl 12(%esp), %eax     // value
	outw %ax, %dx           // write value to port
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Read a single word from io port
.global _io_port_read_word
.type _io_port_read_word, @function
_io_port_read_word:
	push %ebp
	movl %esp, %ebp
	push %eax
	push %edx
	movl 8(%esp), %edx      // port id
	inw %dx, %ax            // read value from %dx and store in %ax
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Write a single long to io port
.global _io_port_write_long
.type _io_port_write_long, @function
_io_port_write_long:
	push %ebp
	movl %esp, %ebp
	push %eax
	push %edx
	movl 8(%esp), %edx      // port id
	movl 12(%esp), %eax     // value
	outl %eax, %dx         // write value to port
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Read a single word from io port
.global _io_port_read_long
.type _io_port_read_long, @function
_io_port_read_long:
	push %ebp
	movl %esp, %ebp
	push %eax
	push %edx
	movl 8(%esp), %edx      // port id
	inl %dx, %eax          // read value and store in return register
	pop %edx
	pop %eax
	movl %ebp, %esp
	pop %ebp
	ret

// Read a single word from io port
.global _io_wait
.type _io_wait, @function
_io_wait:
	push %ebp
	movl %esp, %ebp
	outb %al, $0x80         // Dummy write to port 0x80
	movl %ebp, %esp
	pop %ebp
	ret