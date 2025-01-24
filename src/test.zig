const serial = @cImport({
    @cInclude("/home/aidanb/Greenwood-OS/include/serial.h");
});

export fn zig_test() void {
    serial.print_serial(@constCast("ZIG TEST IN KERNEL!!!\n"));
}
