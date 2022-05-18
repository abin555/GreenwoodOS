OUTPUT_FORMAT(binary)
ENTRY(main)
SECTIONS{
    . = 0x01000000;
    .text ALIGN(0x2000):
    {
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
}