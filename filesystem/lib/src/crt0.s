.section .text

argc: .long 0
argv: .long 0

.global _start
_start:
	# Set up end of the stack frame linked list.
    mov %eax, argc
    mov %ebx, argv
	# Prepare signals, memory allocation, stdio and such.
	# call initialize_standard_library
	call init_stdlib
    call init_stdio

	# Run the global constructors.
	call _init

	# Restore argc and argv.

	# Run main
	pushl argv
    pushl argc
    call main
    # jmp main

	call clean_stdlib

	# Terminate the process with the exit code.
	ret
.size _start, . - _start
