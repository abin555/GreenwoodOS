OUTPUT_FORMAT(elf32-i386)
ENTRY(loader)
SECTIONS {
	. = 0xC0000000;
	kernel_start = .;
	kernel_physical_start = . - 0xC0000000;
	.text ALIGN (0x2000) : AT(ADDR(.text)-0xC0000000)
	{
		*(.multiboot)
		*(.text)
	}
	.rodata ALIGN (0x2000) : AT(ADDR(.rodata)-0xC0000000)
	{
		*(.rodata*)
	}
	.data ALIGN (0x2000) : AT(ADDR(.data)-0xC0000000)
	{
		*(COMMON)
		*(.data)
	}
	.bss ALIGN (0x2000) : AT(ADDR(.bss)-0xC0000000)
	{
		*(.bss)
	}
	kernel_end = .;
	kernel_physical_end = . - 0xC0000000;
}	
