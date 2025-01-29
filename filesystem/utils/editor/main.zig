const std = @import("std");
const libc = @import("libc.zig");
const Window = @import("window.zig");
const Text = @import("text.zig");
const Editor = @import("editor.zig");

const heap_size: usize = 2 * 0x400000;
pub var global_heap: *anyopaque = undefined;

export fn main(argc: c_int, argv: [*c][*c]u8) void {
    if (argc != 2) return;
    const filename = argv[1];
    if (libc.requestRegion(heap_size)) |heap| {
        global_heap = heap;
    }
    @call(.auto, main_zig, .{filename});
    libc.freeRegion(global_heap, heap_size);
}

pub var running = true;

export fn event_handler(vp: *libc.Viewport, vp_event: libc.VIEWPORT_EVENT_TYPE) void {
    _ = vp;
    if (vp_event == libc.VP_EXIT) {
        running = false;
    }
}

pub fn main_zig(filename: [*c]u8) void {
    libc.dprint(@constCast("ZIG Editor Opening\n"));
    libc.dprint(filename);
    var fba = std.heap.FixedBufferAllocator.init(@as([*]u8, @ptrCast(global_heap))[0..heap_size]);
    defer fba.reset();
    const allocator = fba.allocator();

    const window = Window.open(8 * 50, 8 * 30, "Zig Editor", allocator);
    Window.add_event_handler(window, &event_handler);
    Window.clear(window, 0);
    Window.setPixel(window, 75, 50);
    const mainFont = Text.getOSFont();
    Text.putChar(window, mainFont, 8, 8, 'C', 0xFF00FF, 0x00FF00);
    Window.show(window);

    const editor: Editor.Editor = Editor.openEditor(filename, allocator);

    Editor.runEditor(window, editor, mainFont, &running);

    Window.close(window);
}
