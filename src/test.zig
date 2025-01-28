const serial = @cImport({
    @cInclude("serial.h");
});

export fn zig_test() void {
    serial.print_serial(@constCast("ZIG TEST IN KERNEL!!!\n"));
}
