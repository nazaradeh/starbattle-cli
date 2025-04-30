const std = @import("std");
const print = std.debug.print;

const globals = @import("globals.zig");
const generate_puzzle = @import("generate_puzzle.zig");
const grid = @import("grid.zig");

pub fn main() !void {
    try generate_puzzle.generatePuzzle();
    print("{}\n", .{globals.board.regions[2][2]});
}
