OBJECTS = \
		boot.o
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
	qemu-system-x86_64 -boot d -cdrom GreenwoodOS.img -m 512 -monitor stdio \
	-vga std \
	-device ahci,id=ahci \
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
	rm -rf *.o
	rm -rf ./DRIVERS/*.o
	rm -rf ./DRIVERS/usb/*.o

build_clean: os.iso transfer-compiled
build_clean_run: os.iso transfer-compiled run