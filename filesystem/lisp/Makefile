FILES := src/main.c src/greenwood-lisp.c src/builtin.c
CFLAGS_linux := -I ./include -ledit -g
CFLAGS_gwos := -I ./include -DGreenwood_OS -g -c -O0 -m32 -nostdlib -nodefaultlibs -fcf-protection=none -fno-builtin -fno-stack-protector -masm=intel
LDFLAGS = -T ./GWOS_lib/link.ld -melf_i386
CC := i386-elf-gcc

OUTDIR = ./bin/
OUTPUT = greenwoodLISP

build:
	$(CC) $(CFLAGS_gwos) src/main.c -o main.o
	$(CC) $(CFLAGS_gwos) src/greenwood-lisp.c -o greenwood-lisp.o
	$(CC) $(CFLAGS_gwos) src/builtin.c -o builtin.o
	$(CC) $(CFLAGS_gwos) src/gwos_lib.c -o gwos_lib.o
	$(CC) $(CFLAGS_gwos) GWOS_lib/libc.c -o libc.o
	ld $(LDFLAGS) main.o greenwood-lisp.o builtin.o gwos_lib.o libc.o -o LISP.elf
	objcopy -O binary --only-section=.text LISP.elf LISP.exe
	rm *.o

default: build

