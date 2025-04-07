CFLAGS := -I ./include -g -c
CC := i386-gwos-gcc

OUTDIR = ./bin/
OUTPUT = greenwoodLISP

build:
	$(CC) $(CFLAGS) src/main.c -o main.o
	$(CC) $(CFLAGS) src/greenwood-lisp.c -o greenwood-lisp.o
	$(CC) $(CFLAGS) src/builtin.c -o builtin.o
	$(CC) main.o greenwood-lisp.o builtin.o -o LISP.elf
	rm *.o

default: build

