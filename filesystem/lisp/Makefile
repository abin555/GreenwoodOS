CFLAGS := -I ./include -g -c
CC := i386-gwos-gcc

OUTDIR = ./bin/
OUTPUT = greenwoodLISP

build:
	$(CC) $(CFLAGS) src/main.c -o main.o
	$(CC) $(CFLAGS) src/lisp.c -o lisp.o
	$(CC) main.o lisp.o -o lisp.elf
	rm *.o

default: build

