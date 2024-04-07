OBJECTS = \
		src/multiboot.o \
		src/boot.o \
		src/main.o \
		src/framebuffer.o \
		src/serial.o \
		src/io.o \
		src/io_asm.o \
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
		src/drivers/elf.o
		
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -I./src/interfaces -I./src/drivers -I. -masm=intel -g -c
LDFLAGS = -T link.ld -melf_i386 --allow-multiple-definition
AS = nasm
ASFLAGS = -f elf -gdwarf

all: kernel.elf transfer-compiled make_fs

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
	#~/copy_bin/ln $(LDFLAGS) $(OBJECTS) -o kernel.elf
	cd ..
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o GreenwoodOS.iso iso
run: os.iso transfer-compiled emulate

DEBUG_EMU = 

emulate:
	qemu-system-i386 $(DEBUG_EMU) -boot order=c -m 512 -monitor stdio -serial file:serial.log \
	-device ich9-intel-hda \
	-audiodev pa,id=snd0 \
	-device hda-output,audiodev=snd0 \
	-drive id=disk,file=GreenwoodOS.iso,if=none,format=raw \
	-drive id=disk2,file=filesystem.iso,if=none,format=raw \
	-device ahci,id=ahci \
	-device ide-hd,drive=disk2,bus=ahci.0,bootindex=2 \
	-device ide-hd,drive=disk,bus=ahci.1,bootindex=1

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -rf src/*.o src/*.i src/main.s *\~  kernel.elf GreenwoodOS.iso
cleanup:
	rm -rf src/*.o src/*.i src/main.s *\~ src/*/*.o ./*/*.o
build: os.iso transfer-compiled debug #make_fs
debug: build make_fs
	objcopy --only-keep-debug kernel.elf kernel.sym
	objcopy --strip-debug kernel.elf
transfer-compiled:
	rm -rf src/*.o
	rm -rf src/interfaces/*.o
	rm -rf src/drivers/*.o
	rm -rf programs/*.o
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
