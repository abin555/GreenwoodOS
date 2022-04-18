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
	. = 0x01000000;
	.PROGA ALIGN (0x2000):
	{
		*(.PROGA)	
	}
	. = 0x02000000;
	.PROGB ALIGN (0x2000):
	{
		*(.PROGB)
	}
}	
