OBJECTS = \
		src/multiboot.o \
		src/boot.o \
		src/main.o \
		src/framebuffer.o \
		src/serial.o \
		src/io.o \
		src/cpu.o \
		src/gdt.o \
		src/paging_asm.o \
		src/paging.o \
		src/interrupts.o \
		src/interrupt_asm.o \
		src/memory.o \
		src/allocator.o \
		src/grub.o \
		src/pci.o \
		src/timer.o \
		src/cpu_asm.o \
		src/ps2.o \
		src/interfaces/keyboard.o \
		src/interfaces/mouse.o \
		src/multitasking.o \
		src/drivers/ahci.o \
		src/drivers/drive.o \
		src/drivers/FAT32.o \
		src/drivers/ISO9660.o \
		src/drivers/ext2.o \
		src/program.o \
		src/window.o \
		src/console.o \
		src/system_calls.o \
		src/font.o \
		src/drivers/random.o \
		src/exceptions.o \
		src/utils.o \
		src/drivers/elf.o \
		src/drivers/pcspeaker.o \
		src/desktop/desktop.o \
		src/desktop/bitmap.o \
		src/desktop/gfx.o \
		src/desktop/icon.o \
		src/desktop/viewport.o \
		src/stacktrace.o \
		src/rtc.o \
		src/drivers/hda.o \
		src/drivers/intel_e1000.o \
		src/drivers/rtl8139.o \
		src/networking/ethernet.o \
		src/networking/network_utils.o \
		src/test.o
		
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -I./src/interfaces -I./src/drivers -I./src/desktop -I./src/networking/ -I. -masm=intel -g -c
LDFLAGS = -T link.ld -melf_i386 --allow-multiple-definition
AS = nasm
ASFLAGS = -f elf -gdwarf

ZIGC = zig build-obj
ZIGC_FLAGS = -target x86-freestanding -I ./include/

all: build emulate

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
	cd ..	
	objcopy --only-keep-debug kernel.elf kernel.sym
	objcopy --strip-debug kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o GreenwoodOS.iso iso
run: os.iso emulate

DEBUG_EMU = 

emulate:
	./emulate.sh

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
%.o: %.zig
	$(ZIGC) $< $(ZIGC_FLAGS) -femit-bin=$@

clean:
	rm -rf src/*.i src/main.s *\~  kernel.elf GreenwoodOS.iso
	rm -rf $(OBJECTS)

build: os.iso debug #make_fs


debug: build make_fs

make_fs:
	mkisofs -o ./filesystem.iso ./filesystem/
	#dd if=/dev/zero of=filesystem.iso bs=1024 count=1024000
	#mkfs.vfat filesystem.iso
	#mcopy -v ./filesystem/* ./filesystem.iso

make_ext2:
	truncate -s 1G fstest.img
	mkfs.ext2 -r 1 -b 1024 fstest.img
	mkdir fs_ext2
	sudo mount -t ext2 -o loop fstest.img fs_ext2
	sudo cp -r ./filesystem/* fs_ext2/
	sudo umount fs_ext2
	rmdir fs_ext2
