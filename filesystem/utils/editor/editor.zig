const std = @import("std");
const libc = @import("libc.zig");
const Window = @import("window.zig");
const Text = @import("text.zig");

pub const Editor = struct {
    file: [*c]libc.FILE,
    filename: [*c]u8,
    alloc: std.mem.Allocator,
    file_buffer: []u8,
    file_size: i32,
};

pub fn openEditor(filename: [*c]u8, alloc: std.mem.Allocator) !Editor {
    var editor: Editor = undefined;
    libc.dprint(@constCast("Checking if exists\n"));
    if (libc.fexists(filename) != libc.true) return editor;
    editor.filename = filename;
    editor.alloc = alloc;
    editor.file = libc.fopen(editor.filename);
    libc.dprint(@constCast("File Opened?\n"));
    editor.file_size = @as(i32, libc.fsize(editor.file));
    libc.print_arg(@constCast("File size: %d\n"), editor.file_size);
    libc.dprint(@constCast("Allocating the file\n"));
    editor.file_buffer = editor.alloc.alloc(u8, @intCast(editor.file_size));

    return editor;
}

pub fn runEditor(window: Window.Window, editor: Editor, font: Text.FONT, running: *bool) void {
    while (running.*) {
        Text.putChar(window, font, 0, 0, editor.file_buffer[0], 0xFFFFFF, 0x0);
        Window.show(window);
    }
}
