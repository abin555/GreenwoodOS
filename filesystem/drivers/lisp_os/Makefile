CFLAGS := -fPIC -Wl,-q
CC := i386-gwos-gcc

OUTDIR = ./bin/
OUTPUT = greenwoodLISP

build:
	$(CC) $(CFLAGS) main.c -o os.elf

default: build

