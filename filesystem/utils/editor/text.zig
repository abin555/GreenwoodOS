const std = @import("std");
const libc = @import("libc.zig");
const Window = @import("window.zig");

pub const FONT = struct { bitmap: [*c][128][8]u8, size: u32 };

pub fn getOSFont() FONT {
    var osFont: FONT = undefined;
    const query = libc.getKernelFeature(libc.FEAT_FONT);
    if (query.addr) |addr| {
        osFont.bitmap = @as([*c][128][8]u8, @ptrCast(@alignCast(addr)));
    }
    osFont.size = query.size;
    return osFont;
}

pub fn putChar(window: Window.Window, font: FONT, x: u32, y: u32, char: u8, fg_color: u32, bg_color: u32) void {
    const w: u32 = @intCast(@as(i32, window.Viewport.loc.w));
    for (0..8) |layer| {
        for (0..8) |pixel| {
            const offset: usize = w * (y + layer) + x + pixel;
            const color: bool = (@as(c_int, @bitCast(@as(c_uint, font.bitmap.*[@as(c_uint, @intCast(@as(c_int, @bitCast(@as(c_uint, char)))))][@as(c_uint, @intCast(layer))]))) >> @intCast(pixel)) & @as(c_int, 1) != 0;
            if (color) {
                window.buffer[offset] = fg_color;
            } else {
                window.buffer[offset] = bg_color;
            }
        }
    }
}
