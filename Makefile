OBJECTS = \
		boot.o \
		main.o \
		io.o \
		frame_buffer.o \
		ascii_tables.o \
		interrupts.o \
		DRIVERS/keyboard.o \
		kernel_programs/terminal.o \
		kernel_programs/keyboard_test.o \
		pic.o \
		string.o\
		memory.o\
		gfx.o\
		kernel_programs/pong.o\
		kernel_programs/grapher.o\
		DRIVERS/PCI.o\
		DRIVERS/usb.o\
		drivers.o\
		DRIVERS/IDE.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -save-temps -masm=intel -c
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
run: os.iso transfer-compiled
	qemu-system-x86_64 -boot d -cdrom GreenwoodOS.img -m 512 -monitor stdio \
	-vga std \
	-drive if=none,id=usbstick,format=raw,file=GreenwoodOS.img   \
	-usb \
	-device usb-ehci,id=ehci

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
	rm -rf ./DRIVERS/*.o
	cp ./compile/boot.s .

build_clean: os.iso transfer-compiled
build_clean_run: os.iso transfer-compiled run