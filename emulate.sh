qemu-system-i386 -s -boot order=c -m 2G -monitor stdio -serial file:serial.log \
-device ich9-intel-hda \
-drive id=disk,file=./GreenwoodOS.iso,if=none,format=raw \
-drive id=disk3,file=fstest.img,if=none,format=raw \
-drive id=disk2,file=filesystem.iso,if=none,format=raw \
-device ahci,id=ahci \
-device ide-hd,drive=disk3,bus=ahci.0,bootindex=2 \
-device ide-hd,drive=disk,bus=ahci.1,bootindex=1 \
-device ide-hd,drive=disk2,bus=ahci.2,bootindex=3
