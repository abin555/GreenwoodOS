const libc = @cImport({
    @cInclude("/home/aidanb/Greenwood-OS/filesystem/OS/libc/libc.h");
});

export fn main() void {
    @call(.auto, main_zig, .{});
}

pub var running = true;

export fn event_handler(vp: *libc.Viewport, vp_event: libc.VIEWPORT_EVENT_TYPE) void {
    _ = vp;
    if (vp_event == libc.VP_EXIT) {
        running = false;
    }
}

pub fn main_zig() void {
    const msg: [*c]const u8 = "ZIG Test\n";
    libc.dprint(@constCast(msg));
    const vp_functions_ptr = libc.viewport_get_funcs();
    libc.dprint(@constCast("Got viewport?\n"));
    const vp_funcs = vp_functions_ptr.*;
    var vp: [*c]libc.Viewport = undefined;
    if (vp_funcs.open) |open| {
        vp = open(150, 150, @constCast("ZIG PROGRAM"));
        libc.dprint(@constCast("Open?\n"));
    } else {
        libc.dprint(@constCast("Open failed!\n"));
        return;
    }
    if (vp_funcs.add_event_handler) |add_event| {
        add_event(vp, @ptrCast(&event_handler));
    } else {
        return;
    }
    while (running) {}
    if (vp_funcs.close) |close| {
        close(vp);
    } else {
        return;
    }
}
