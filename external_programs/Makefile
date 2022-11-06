OBJECTS = \
	./test.o

AS = nasm
ASFLAGS = 
LDFLAGS = -T link.ld

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
	ld $(LDFLAGS) $< -o $<.bin
