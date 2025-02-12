const std = @import("std");
const builtin = @import("builtin");
const os_tag = builtin.os.tag;
const arch = builtin.cpu.arch;
const abi = builtin.abi;
const is_test = builtin.is_test;

const linkage: std.builtin.GlobalLinkage = if (builtin.is_test) .internal else .weak;

// Zig's own stack-probe routine (available only on x86 and x86_64)
pub fn zig_probe_stack() callconv(.Naked) void {
    @setRuntimeSafety(false);

    // Versions of the Linux kernel before 5.1 treat any access below SP as
    // invalid so let's update it on the go, otherwise we'll get a segfault
    // instead of triggering the stack growth.

    switch (arch) {
        .x86_64 => {
            // %rax = probe length, %rsp = stack pointer
            asm volatile (
                \\        push   %%rcx
                \\        mov    %%rax, %%rcx
                \\        cmp    $0x1000,%%rcx
                \\        jb     2f
                \\ 1:
                \\        sub    $0x1000,%%rsp
                \\        orl    $0,16(%%rsp)
                \\        sub    $0x1000,%%rcx
                \\        cmp    $0x1000,%%rcx
                \\        ja     1b
                \\ 2:
                \\        sub    %%rcx, %%rsp
                \\        orl    $0,16(%%rsp)
                \\        add    %%rax,%%rsp
                \\        pop    %%rcx
                \\        ret
            );
        },
        .x86 => {
            // %eax = probe length, %esp = stack pointer
            asm volatile (
                \\        push   %%ecx
                \\        mov    %%eax, %%ecx
                \\        cmp    $0x1000,%%ecx
                \\        jb     2f
                \\ 1:
                \\        sub    $0x1000,%%esp
                \\        orl    $0,8(%%esp)
                \\        sub    $0x1000,%%ecx
                \\        cmp    $0x1000,%%ecx
                \\        ja     1b
                \\ 2:
                \\        sub    %%ecx, %%esp
                \\        orl    $0,8(%%esp)
                \\        add    %%eax,%%esp
                \\        pop    %%ecx
                \\        ret
            );
        },
        else => {},
    }

    unreachable;
}

comptime {
    switch (arch) {
        .x86,
        .x86_64,
        => {
            @export(zig_probe_stack, .{ .name = "__zig_probe_stack", .linkage = linkage });
        },
        else => {},
    }
}
