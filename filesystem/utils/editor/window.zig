const libc = @import("libc.zig");
const std = @import("std");

pub const Window = struct { Viewport: *libc.struct_Viewport, vp_functions: libc.struct_ViewportFunctions, buffer: []u32, alloc: std.mem.Allocator };

pub fn open(width: u32, height: u32, name: []const u8, alloc: std.mem.Allocator) Window {
    var win: Window = undefined;
    win.vp_functions = libc.viewport_get_funcs().*;
    if (win.vp_functions.open) |vp_open| {
        win.Viewport = vp_open(@intCast(width), @intCast(height), @constCast(name.ptr));
    }
    if (win.vp_functions.set_buffer) |set_buffer| {
        const buf = alloc.alloc(u32, width * height) catch unreachable;
        win.buffer = buf;
        set_buffer(win.Viewport, @ptrCast(buf), @sizeOf(u32) * width * height);
    }
    return win;
}

pub fn close(win: Window) void {
    if (win.vp_functions.close) |vp_close| {
        vp_close(win.Viewport);
    }
}

pub fn add_event_handler(win: Window, function: *const fn (*libc.struct_Viewport, libc.VIEWPORT_EVENT_TYPE) callconv(.C) void) void {
    if (win.vp_functions.add_event_handler) |add_handler| {
        add_handler(win.Viewport, @ptrCast(function));
    }
}

pub fn setPixel(win: Window, x: u32, y: u32) void {
    libc.dprint(@constCast("Check\n"));
    const w: u32 = @intCast(@as(i32, win.Viewport.loc.w));
    libc.dprint(@constCast("Check\n"));
    const offset: u32 = ((w * y) + x);
    libc.dprint(@constCast("Check\n"));
    win.buffer[offset] = 0xFFFFFF;
    libc.dprint(@constCast("Check\n"));
}

pub fn show(win: Window) void {
    if (win.vp_functions.copy) |copy| {
        copy(win.Viewport);
    }
}
