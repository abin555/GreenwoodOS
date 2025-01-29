pub const @"bool" = u8;
pub const struct_IVec2 = extern struct {
    x: c_int = @import("std").mem.zeroes(c_int),
    y: c_int = @import("std").mem.zeroes(c_int),
};
// ../../OS/libc/libc.h:25:13: warning: struct demoted to opaque type - has bitfield
pub const struct_MouseButtons = opaque {};
pub const struct_MouseStatus = extern struct {
    pos: struct_IVec2 = @import("std").mem.zeroes(struct_IVec2),
    buttons: struct_MouseButtons = @import("std").mem.zeroes(struct_MouseButtons),
    lastDelta: struct_IVec2 = @import("std").mem.zeroes(struct_IVec2),
};
pub const struct_PCSpeaker_Handle = extern struct {
    play: ?*const fn (u32) callconv(.C) void = @import("std").mem.zeroes(?*const fn (u32) callconv(.C) void),
    mute: ?*const fn () callconv(.C) void = @import("std").mem.zeroes(?*const fn () callconv(.C) void),
    beep: ?*const fn () callconv(.C) void = @import("std").mem.zeroes(?*const fn () callconv(.C) void),
};
pub const struct_WINDOW = extern struct {
    backbuffer: [*c]u32 = @import("std").mem.zeroes([*c]u32),
    name: [21]u8 = @import("std").mem.zeroes([21]u8),
    active: @"bool" = @import("std").mem.zeroes(@"bool"),
    width: u32 = @import("std").mem.zeroes(u32),
    height: u32 = @import("std").mem.zeroes(u32),
    copyOnPromptOnly: @"bool" = @import("std").mem.zeroes(@"bool"),
};
pub const ALWAYS: c_int = 1;
pub const ONFOCUS: c_int = 2;
pub const NOCHILD: c_int = 3;
pub const NEVER: c_int = 4;
pub const ScheduleType = c_uint;
const struct_unnamed_1 = extern struct {
    fg: u32 = @import("std").mem.zeroes(u32),
    bg: u32 = @import("std").mem.zeroes(u32),
};
pub const struct_CONSOLE = extern struct {
    window: [*c]struct_WINDOW = @import("std").mem.zeroes([*c]struct_WINDOW),
    width: u32 = @import("std").mem.zeroes(u32),
    height: u32 = @import("std").mem.zeroes(u32),
    buf_size: u32 = @import("std").mem.zeroes(u32),
    buf: [*c]u8 = @import("std").mem.zeroes([*c]u8),
    active: @"bool" = @import("std").mem.zeroes(@"bool"),
    last_cursor: c_int = @import("std").mem.zeroes(c_int),
    cursor: c_int = @import("std").mem.zeroes(c_int),
    Line: u32 = @import("std").mem.zeroes(u32),
    Start: u32 = @import("std").mem.zeroes(u32),
    LinePlace: u32 = @import("std").mem.zeroes(u32),
    LastLine: u32 = @import("std").mem.zeroes(u32),
    color: struct_unnamed_1 = @import("std").mem.zeroes(struct_unnamed_1),
};
pub const struct_File_Info = extern struct {
    drive: ?*anyopaque = @import("std").mem.zeroes(?*anyopaque),
    sector: u32 = @import("std").mem.zeroes(u32),
    size: u32 = @import("std").mem.zeroes(u32),
};
pub const struct_FILE = extern struct {
    info: struct_File_Info = @import("std").mem.zeroes(struct_File_Info),
    head: c_int = @import("std").mem.zeroes(c_int),
};
pub extern fn strcmp(s1: [*c]const u8, s2: [*c]const u8) c_int;
pub extern fn window_open(name: [*c]u8, copyOnPrompt: u32) [*c]struct_WINDOW;
pub extern fn window_close(window: [*c]struct_WINDOW) void;
pub extern fn window_update(...) void;
pub extern fn drawChar(x: u32, y: u32, c: u8) void;
pub extern fn getc(...) u8;
pub extern fn exec(filename: [*c]u8, argc: c_int, argv: [*c][*c]u8) void;
pub extern fn set_schedule(@"type": ScheduleType) void;
pub extern fn print(str: [*c]u8) void;
pub extern fn print_arg(str: [*c]u8, arg: u32) void;
pub extern fn print_float(str: [*c]u8, arg: f32) void;
pub extern fn console_open(...) [*c]struct_CONSOLE;
pub extern fn console_close(...) void;
pub extern fn kmalloc(size: u32) ?*anyopaque;
pub extern fn fopen(file: [*c]u8) [*c]struct_FILE;
pub extern fn fclose(file: [*c]struct_FILE) void;
pub extern fn fgetc(file: [*c]struct_FILE) u8;
pub extern fn fsize(file: [*c]struct_FILE) c_int;
pub extern fn fcopy(file: [*c]struct_FILE, buf: [*c]u8, buf_size: c_int) c_int;
pub extern fn changeDirectory(change: [*c]u8) c_int;
pub extern fn getDirectory(...) [*c]u8;
pub extern fn printDirectoryContents(path: [*c]u8) void;
pub extern fn fexists(path: [*c]u8) c_int;
pub const FEAT_FONT: c_int = 1;
pub const FEAT_KEYBOARD: c_int = 2;
pub const FEAT_PAGETABLE: c_int = 3;
pub const FEAT_TASKTABLE: c_int = 4;
pub const FEAT_PROGRAMBASE: c_int = 5;
pub const FEAT_FRAMEBUFFER: c_int = 6;
pub const FEAT_TIMER: c_int = 7;
pub const FEAT_KEYPRESSMAP: c_int = 8;
pub const KERNEL_FEATURE = c_uint;
pub const struct_FEATURE_INFO = extern struct {
    addr: ?*anyopaque = @import("std").mem.zeroes(?*anyopaque),
    size: u32 = @import("std").mem.zeroes(u32),
};
pub extern fn getKernelFeature(feature: KERNEL_FEATURE) struct_FEATURE_INFO;
pub extern fn fseek(file: [*c]struct_FILE, idx: c_int) void;
pub extern fn fputc(file: [*c]struct_FILE, c: u8) void;
pub extern fn fmkdir(path: [*c]u8) void;
pub extern fn fmkfile(path: [*c]u8, size: c_int) void;
pub extern fn fextend(file: [*c]struct_FILE, amount: u32) c_int;
pub extern fn getArrowKey(...) c_int;
pub extern fn srand(seed: u32) void;
pub extern fn rand(...) c_int;
pub extern fn exit(code: c_int) noreturn;
pub extern fn requestRegion(bytes: c_uint) ?*anyopaque;
pub extern fn addKbdEventHandler(handler: ?*const fn (u8) callconv(.C) void) void;
pub extern fn requestRegionAt(bytes: c_uint, addr: c_uint) ?*anyopaque;
pub extern fn attachTimerCallback(timer: c_uint, callback: ?*anyopaque) void;
pub extern fn dprint(msg: [*c]u8) void;
pub extern fn start_manual_task(addr: ?*anyopaque, name: [*c]u8) void;
pub extern fn getMouse(...) ?*struct_MouseStatus;
pub extern fn getPCSpeaker(...) [*c]struct_PCSpeaker_Handle;
pub extern fn getTimerTickHandle(...) [*c]u32;
pub extern fn addMouseEventHandler(handler: ?*const fn () callconv(.C) void) void;
pub extern fn freeRegion(addr: ?*anyopaque, size: u32) void;
pub extern fn addEndCallback(handler: ?*const fn () callconv(.C) void) void;
pub const struct_Location = extern struct {
    x: c_int = @import("std").mem.zeroes(c_int),
    y: c_int = @import("std").mem.zeroes(c_int),
    w: c_int = @import("std").mem.zeroes(c_int),
    h: c_int = @import("std").mem.zeroes(c_int),
};
pub const VP_FOCUSED: c_int = 0;
pub const VP_UNFOCUSED: c_int = 1;
pub const VP_KEY: c_int = 2;
pub const VP_EXIT: c_int = 3;
pub const VP_MINIMIZE: c_int = 4;
pub const VP_MAXIMIZE: c_int = 5;
pub const VP_CLICK: c_int = 6;
pub const VIEWPORT_EVENT_TYPE = c_uint;
pub const struct_Viewport = extern struct {
    loc: struct_Location = @import("std").mem.zeroes(struct_Location),
    oldLoc: struct_Location = @import("std").mem.zeroes(struct_Location),
    frontbuf: [*c]u32 = @import("std").mem.zeroes([*c]u32),
    backbuf: [*c]u32 = @import("std").mem.zeroes([*c]u32),
    buf_size: u32 = @import("std").mem.zeroes(u32),
    resizeable: u8 = @import("std").mem.zeroes(u8),
    minimized: @"bool" = @import("std").mem.zeroes(@"bool"),
    open: @"bool" = @import("std").mem.zeroes(@"bool"),
    minimized_w: c_int = @import("std").mem.zeroes(c_int),
    minimized_h: c_int = @import("std").mem.zeroes(c_int),
    title: [*c]u8 = @import("std").mem.zeroes([*c]u8),
    owner_program_slot: c_int = @import("std").mem.zeroes(c_int),
    owner_task_id: c_int = @import("std").mem.zeroes(c_int),
    ascii: u8 = @import("std").mem.zeroes(u8),
    event_handler: ?*const fn ([*c]struct_Viewport, VIEWPORT_EVENT_TYPE) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport, VIEWPORT_EVENT_TYPE) callconv(.C) void),
    click_events_enabled: @"bool" = @import("std").mem.zeroes(@"bool"),
};
pub const struct_ViewportFunctions = extern struct {
    open: ?*const fn (c_int, c_int, [*c]u8) callconv(.C) [*c]struct_Viewport = @import("std").mem.zeroes(?*const fn (c_int, c_int, [*c]u8) callconv(.C) [*c]struct_Viewport),
    close: ?*const fn ([*c]struct_Viewport) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport) callconv(.C) void),
    set_buffer: ?*const fn ([*c]struct_Viewport, [*c]u32, u32) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport, [*c]u32, u32) callconv(.C) void),
    copy: ?*const fn ([*c]struct_Viewport) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport) callconv(.C) void),
    add_event_handler: ?*const fn ([*c]struct_Viewport, ?*const fn ([*c]struct_Viewport, VIEWPORT_EVENT_TYPE) callconv(.C) void) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport, ?*const fn ([*c]struct_Viewport, VIEWPORT_EVENT_TYPE) callconv(.C) void) callconv(.C) void),
    drawChar: ?*const fn ([*c]struct_Viewport, c_int, c_int, u8, u32, u32) callconv(.C) void = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport, c_int, c_int, u8, u32, u32) callconv(.C) void),
    getc: ?*const fn ([*c]struct_Viewport) callconv(.C) u8 = @import("std").mem.zeroes(?*const fn ([*c]struct_Viewport) callconv(.C) u8),
};
pub extern fn viewport_get_funcs(...) [*c]struct_ViewportFunctions;
pub extern fn console_open_vp(vp: [*c]struct_Viewport) [*c]struct_CONSOLE;
pub const struct_RealTimeClock = extern struct {
    second: u8 = @import("std").mem.zeroes(u8),
    minute: u8 = @import("std").mem.zeroes(u8),
    hour: u8 = @import("std").mem.zeroes(u8),
    day: u8 = @import("std").mem.zeroes(u8),
    month: u8 = @import("std").mem.zeroes(u8),
    year: c_uint = @import("std").mem.zeroes(c_uint),
    msec: c_uint = @import("std").mem.zeroes(c_uint),
};
pub extern fn get_rtc(...) [*c]struct_RealTimeClock;
pub const struct_DirectoryEntry = extern struct {
    filename: [50]u8 = @import("std").mem.zeroes([50]u8),
    name_len: c_int = @import("std").mem.zeroes(c_int),
    type: u32 = @import("std").mem.zeroes(u32),
};
pub const struct_DirectoryListing = extern struct {
    directory_path: [*c]u8 = @import("std").mem.zeroes([*c]u8),
    directory_path_len: c_int = @import("std").mem.zeroes(c_int),
    entries: [*c]struct_DirectoryEntry = @import("std").mem.zeroes([*c]struct_DirectoryEntry),
    num_entries: c_int = @import("std").mem.zeroes(c_int),
};
pub extern fn getDirectoryListing(path: [*c]u8) struct_DirectoryListing;
pub extern fn print_serial(str: [*c]u8) void;
pub extern fn task_lock(state: c_int) void;
pub extern fn write_serial(c: u8) void;
pub extern fn yield(...) void;
pub const @"true" = @as(c_int, 1);
pub const @"false" = @as(c_int, 0);
pub const NULL = @as(c_int, 0);
pub const IVec2 = struct_IVec2;
pub const MouseButtons = struct_MouseButtons;
pub const MouseStatus = struct_MouseStatus;
pub const PCSpeaker_Handle = struct_PCSpeaker_Handle;
pub const WINDOW = struct_WINDOW;
pub const CONSOLE = struct_CONSOLE;
pub const File_Info = struct_File_Info;
pub const FILE = struct_FILE;
pub const FEATURE_INFO = struct_FEATURE_INFO;
pub const Location = struct_Location;
pub const Viewport = struct_Viewport;
pub const ViewportFunctions = struct_ViewportFunctions;
pub const RealTimeClock = struct_RealTimeClock;
pub const DirectoryEntry = struct_DirectoryEntry;
pub const DirectoryListing = struct_DirectoryListing;
