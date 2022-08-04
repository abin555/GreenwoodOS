OUTPUT_FORMAT(elf32-i386)
ENTRY(loader)
SECTIONS {
	. = 0x00100000;
	.text ALIGN (0x2000):
	{
		*(.multiboot)
		*(.text)
	}
	.rodata ALIGN (0x2000):
	{
		*(.rodata*)
	}
	.data ALIGN (0x2000):
	{
		*(.data)
	}
	.bss ALIGN (0x2000):
	{
		*(COMMON)
		*(.bss)
	}
	kernel_end = .;
	heap_start = 0x10000000;
}	
