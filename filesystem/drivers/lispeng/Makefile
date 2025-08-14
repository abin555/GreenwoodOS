CFLAGS := -fPIC
LDFLAGS :=  -Wl,-q 
INCLUDES := -I ./include 
CC := i386-gwos-gcc



OUTDIR = ./bin/
OUTPUT = greenwoodLISP

build:
	$(CC) $(INCLUDES) $(CFLAGS) -c src/main.c -o main.o
	$(CC) $(INCLUDES) $(CFLAGS) -c src/greenwood-lisp.c -o greenwood-lisp.o
	$(CC) $(INCLUDES) $(CFLAGS) -c src/builtin.c -o builtin.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o greenwood-lisp.o builtin.o -o lisp.elf
	rm *.o

default: build

