OBJECTS = \
		src/boot.o \
		src/main.o \
		src/framebuffer.o \
		src/interrupts.o \
		src/pic.o \
		src/io.o \
		src/memory.o \
		src/drivers/console.o \
		src/drivers/ps2.o \
		src/drivers/keyboard.o \
		src/drivers/pci.o \
		src/timer.o \
		src/drivers/ahci.o \
		src/drivers/filesystem.o \
		src/drivers/drivers.o \
		src/syscalls.o \
		programs/terminal.o \
		src/processes.o \
		programs/editor.o \
		programs/art.o \
		src/paging.o \
		src/paging_asm.o \
		src/program_region.o \
		src/programs.o \
		src/processes_asm.o \
		programs/hex.o \
		src/multiboot.o \
		src/gdt.o \
		src/interrupts_asm.o \
		src/io_asm.o \
		src/serial.o
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -I. -masm=intel -g -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf -gdwarf

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
clean:8535
	rm -rf src/*.o src/*.i src/main.s *\~  kernel.elf GreenwoodOS.iso
cleanup:
	rm -rf src/*.o src/*.i src/main.s *\~
build: os.iso transfer-compiled debug
debug: build
	objcopy --only-keep-debug kernel.elf kernel.sym
	objcopy --strip-debug kernel.elf
transfer-compiled:
	cp src/*.o ./compile
	rm -rf src/*.o
	rm -rf src/drivers/*.o
	rm -rf src/drivers/usb/*.o
	rm -rf programs/*.o

build_clean: os.iso transfer-compiled
build_clean_run: os.iso transfer-compiled run