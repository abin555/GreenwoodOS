CC = i386-gwos-gcc
PROG = setbg

SRCS += main.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS)
	i386-gwos-gcc $(SRCS) -o $(PROG).elf

clean:
	rm $(SRCS)