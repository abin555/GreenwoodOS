OBJECTS = \
		src/multiboot.o \
		src/boot.o \
		src/main.o \
		src/framebuffer.o \
		src/serial.o \
		src/io.o \
		src/cpu.o \
		src/gdt.o \
		src/descriptor_table.o \
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
		src/networking/ip.o \
		src/networking/arp.o \
		src/networking/udp.o \
		src/networking/dhcp.o \
		src/networking/tcp.o \
		src/networking/http.o \
		src/networking/netfs.o \
		src/networking/icmp.o \
		src/networking/checksum.o \
		src/networking/netproc.o \
		src/acpi.o \
		src/apic.o \
		src/vfs.o \
		src/drivers/pipe.o \
		src/audio/audio.o \
		src/drivers/ac97.o \
		src/audio/wav.o \
		src/drivers/sysfs.o \
		src/sysroot.o \
		src/audio/audio_cdev.o \
		src/monitors.o \
		src/audio/mic.o \
		src/ap_startup.o \
		src/ap_trampoline.o \
		src/drivers/kernel_fn.o \
		src/drivers/proc.o \
		src/drivers/usb.o \
		src/drivers/ehci.o \
		src/drivers/pcifs.o
		
CC = i386-elf-gcc
CFLAGS = -nostdlib -fno-builtin \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I./src -I./src/interfaces -I./src/drivers -I./src/desktop -I./src/networking/ -I./src/audio/ -I. -I./src/uACPI/include/ -masm=intel -g -c
LDFLAGS = -T link.ld
AS = nasm
ASFLAGS = -f elf -gdwarf

ZIGC = zig build-obj
ZIGC_FLAGS = -target x86-freestanding -I ./include/

all: build emulate

kernel.elf: $(OBJECTS)
	i386-elf-ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
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
	$(CC) $< $(CFLAGS) -o $@
%.o: %.s
	$(AS) $< $(ASFLAGS) -o $@

clean:
	rm -rf src/*.i src/main.s *\~  kernel.elf GreenwoodOS.iso
	rm -rf $(OBJECTS)

build: os.iso #make_fs


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

make_fsos:
	truncate -s 1G fsos.img
	mkfs.ext2 -r 1 -b 1024 fsos.img

fs: make_fs make_ext2