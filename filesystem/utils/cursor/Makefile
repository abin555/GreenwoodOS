CC = i386-gwos-gcc
PROG = cursor

CFLAGS := -masm=intel -g

SRCS += main.o

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

build: $(SRCS)
	i386-gwos-gcc $(SRCS) -g -o $(PROG).elf

clean:
	rm $(SRCS)