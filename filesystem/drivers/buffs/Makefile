CFLAGS := -fPIC -Wl,-q
CC := i386-gwos-gcc

build:
	$(CC) $(CFLAGS) main.c hooks.c buffs.c -o buffs.elf

default: build