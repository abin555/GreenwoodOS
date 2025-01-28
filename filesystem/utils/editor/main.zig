const std = @import("std");
const libc = @import("libc.zig");
const Window = @import("window.zig");

pub var global_heap: *anyopaque = undefined;

export fn main() void {
    if (libc.requestRegion(0x40000)) |heap| {
        global_heap = heap;
    }
    @call(.auto, main_zig, .{});
    libc.freeRegion(global_heap, 0x40000);
}

pub var running = true;

export fn event_handler(vp: *libc.Viewport, vp_event: libc.VIEWPORT_EVENT_TYPE) void {
    _ = vp;
    if (vp_event == libc.VP_EXIT) {
        running = false;
    }
}

pub fn main_zig() void {
    libc.dprint(@constCast("ZIG Editor Opening\n"));
    var fba = std.heap.FixedBufferAllocator.init(@as([*]u8, @ptrCast(global_heap))[0..0x40000]);
    defer fba.reset();
    const allocator = fba.allocator();

    const window = Window.open(150, 150, "Zig Editor", allocator);
    libc.dprint(@constCast("Open\n"));
    Window.add_event_handler(window, &event_handler);
    libc.dprint(@constCast("Event\n"));
    Window.setPixel(window, 50, 50);
    libc.dprint(@constCast("Set Pixel\n"));
    Window.show(window);
    while (running) {}
    Window.close(window);
}
