SECTIONS
{
	. = 0;
	.text :
	{
		*(.text)
		*(.text.*)
	}
	.bss : { *(.bss); *(.bss.*) }
	.data : { *(.data); *(.data.*) }
	.rodata : { *(.rodata); *(.rodata.*) }
	.debug :{
		*(.debug*)
	}
}
