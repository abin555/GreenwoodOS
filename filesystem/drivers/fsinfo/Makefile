CFLAGS := -O0 -g
CC := i386-gwos-gcc

build:
	$(CC) $(CFLAGS) main.c hooks.c -lgwos -o fsinfo.elf

default: build