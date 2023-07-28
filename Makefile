OBJECTS = \
		src/boot.o \
		src/main.o \
		src/framebuffer.o \
		
CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./include -I./src/drivers -I./src/drivers/audio -I./src/drivers/usb -I. -masm=intel -g -O0 -c
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
clean:8535
	rm -rf src/*.o src/*.i src/main.s *\~  kernel.elf GreenwoodOS.iso
cleanup:
	rm -rf src/*.o src/*.i src/main.s *\~ src/*/*.o ./*/*.o
build: os.iso transfer-compiled debug make_fs
debug: build
	objcopy --only-keep-debug kernel.elf kernel.sym
	objcopy --strip-debug kernel.elf
transfer-compiled:
	rm -rf src/*.o
	rm -rf src/drivers/*.o
	rm -rf src/drivers/usb/*.o
	rm -rf src/drivers/audio/*.o
	rm -rf programs/*.o
make_fs:
	mkisofs -o ./filesystem.iso ./filesystem/
