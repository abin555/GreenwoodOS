OBJECTS = \
		boot.o \
		main.o \
		io.o \
		frame_buffer.o \
		ascii_tables.o \
		interrupts.o \
		DRIVERS/keyboard.o \
		DRIVERS/mouse.o \
		kernel_programs/terminal.o \
		kernel_programs/keyboard_test.o \
		pic.o \
		string.o\
		memory.o\
		gfx.o\
		kernel_programs/pong.o\
		kernel_programs/grapher.o\
		DRIVERS/PCI.o\
		DRIVERS/usb/usb.o\
		drivers.o\
		DRIVERS/IDE.o\
		console.o\
		DRIVERS/ahci.o\
		DRIVERS/filesystem.o\
		DRIVERS/usb/ohci.o\
		DRIVERS/usb/uhci.o\
		DRIVERS/usb/ehci.o\
		DRIVERS/usb/xhci.o \
		DRIVERS/ps2.o \
		timer.o \
		paging.o
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -masm=intel -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf transfer-compiled

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
	cd ..
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o GreenwoodOS.img iso
run: os.iso transfer-compiled
	qemu-system-x86_64 -boot d -m 512 -monitor stdio \
	-vga std \
	-drive id=disk,file=GreenwoodOS.img,if=none \
	-device ahci,id=ahci \
	-device ide-hd,drive=disk,bus=ahci.0 \
	-device nec-usb-xhci

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
	rm -rf *.o
	rm -rf ./DRIVERS/*.o
	rm -rf ./DRIVERS/usb/*.o

build_clean: os.iso transfer-compiled
build_clean_run: os.iso transfer-compiled run