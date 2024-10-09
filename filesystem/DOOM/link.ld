SECTIONS
{
	. = 0;
	.text :
	{
		*(.*)
	}
	.bss : { *(.bss); *(.bss.*) }
	.data : { *(.data); *(.data.*) }
	.rodata : { *(.rodata); *(.rodata.*) }
}
