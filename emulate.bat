qemu-system-x86_64 -boot order=c -m 512 -monitor stdio ^
-drive if=none,id=usbstick,format=raw,file=GreenwoodOS.img ^
-drive id=disk,file=GreenwoodOS.img,if=none,format=raw ^
-drive id=disk2,file=GREENWOODOS2.vhd,if=none,format=raw ^
-device ahci,id=ahci ^
-device ide-hd,drive=disk,bus=ahci.0 ^
-device ide-hd,drive=disk2,bus=ahci.1 ^
-usb -device usb-ehci