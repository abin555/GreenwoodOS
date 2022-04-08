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
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o GreenwoodOS.iso \
		iso
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
build: os.iso

build_clean: os.iso cleanup
