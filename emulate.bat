qemu-system-i386 -S -s -boot order=c -m 512 -monitor stdio -serial file:serial.log ^
-drive if=none,id=usbstick,format=raw,file=GreenwoodOS.img ^
-drive id=disk,file=GreenwoodOS.img,if=none,format=raw ^
-drive id=disk2,file=test.iso,if=none,format=raw ^
-device ahci,id=ahci ^
-device ide-hd,drive=disk2,bus=ahci.0,bootindex=2 ^
-device ide-hd,drive=disk,bus=ahci.1,bootindex=1 ^
-device qemu-xhci ^
-device usb-mouse