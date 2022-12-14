AS = nasm
ASFLAGS = -f elf
LDFLAGS = -T link.ld -melf_i386

all:
	$(AS) $(ASFLAGS) flappy.s -o flappy.O
	ld $(LDFLAGS) flappy.o -o flappy.elf
	objdump -d flappy.elf > flappy.txt
	objcopy -O binary --only-section=.text flappy.elf flappy.exe
	rm flappy.o
	rm flappy.elf