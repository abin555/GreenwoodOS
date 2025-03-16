.section .text

.global _start
_start:
	# Set up end of the stack frame linked list.
	push %eax
    push %ebx

	# Prepare signals, memory allocation, stdio and such.
	# call initialize_standard_library
	call init_stdlib
    call init_stdio

	# Run the global constructors.
	call _init

	# Restore argc and argv.
	popl %eax
	popl %eax

	# Run main
	call main

	call clean_stdlib

	# Terminate the process with the exit code.
	ret
.size _start, . - _start
