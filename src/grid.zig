const std = @import("std");
const builtin = @import("builtin");

fn cellElement(row: u8, col: u8) []u8 {
    _ = row;
    _ = col;
}

pub fn updateGrid() void {
    const test_string = "▄▄▄█";
    if (comptime builtin.target.os.tag == .windows) _ = std.os.windows.kernel32.SetConsoleOutputCP(65001);
    std.debug.print(test_string, .{});
}
