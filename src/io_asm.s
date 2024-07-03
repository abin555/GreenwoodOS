;SECTION FOR I/O ASM INTERFACE
;global outb			;Make OUTB function global to C kernel
;outb:
;	mov al, [esp+8] ;Load contents of stack into lower A register | OUTPUT PORT
;	mov dx, [esp+4] ;Load contents of stack into lower B register | OUTPUT DATA
;	out dx, al 		;Send OUTPUT
;	ret				;Return to caller

global inb			;Make INB function global to C kernel
inb:
	mov dx, [esp + 4] 	;Load contents of stack into lower D register | POS
	in al, dx			;Recieve input and put into al register as 8 bit value (C lang Char)
	ret					;Return to caller

global kreboot
kreboot:
	cli
	jmp 0xFFFF:0
	hlt
	hlt
	hlt