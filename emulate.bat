qemu-system-x86_64 -boot order=c -m 512 -monitor stdio ^
-drive if=none,id=usbstick,format=raw,file=GreenwoodOS.img ^
-drive id=disk,file=GreenwoodOS.img,if=none ^
-drive id=disk2,file=iso\test.txt,if=none ^
-device ahci,id=ahci ^
-device ide-hd,drive=disk,bus=ahci.0 ^
-device ide-hd,drive=disk2,bus=ahci.1 ^
-usb -device usb-ehci