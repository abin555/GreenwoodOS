OBJECTS = \
		boot.o \
		main.o \
		serial.o \
		frame_buffer.o \
		io.o interrupts.o \
		pic.o \
		kernel_programs/terminal.o \
		keyboard.o \
		string.o \
		kernel_programs/keyboard_test.o \
		ascii_tables.o 
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -save-temps -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
	cd ..
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o GreenwoodOS.img iso
run: os.iso
	qemu-system-x86_64 -boot d -cdrom GreenwoodOS.iso -m 512 -monitor stdio

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -rf *.o *.i main.s *\~  kernel.elf GreenwoodOS.iso
cleanup:
	rm -rf *.o *.i main.s *\~
build: os.iso transfer-compiled

transfer-compiled:
	cp *.o ./compile
	cp *.i ./compile
	cp *.s ./compile
	rm -rf *.o *.i *.s
	cp ./compile/boot.s .

build_clean: os.iso transfer-compiled
build_clean_run: os.iso transfer-compiled run