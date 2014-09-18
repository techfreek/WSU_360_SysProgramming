/*
 * Name: Alex Bahm
 * File: x86func.s
 */

.global get_esp, get_ebp

get_esp:
	movl %esp, %eax
	ret

get_ebp:
	movl %ebp, %eax
	ret