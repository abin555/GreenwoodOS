CC = i386-gwos-gcc
PROG = crash

SRCS += main.o

%.o : %.c
	$(CC) -c $(CFLAGS) -g $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS)
	i386-gwos-gcc $(SRCS) -g -o $(PROG).elf

clean:
	rm $(SRCS)