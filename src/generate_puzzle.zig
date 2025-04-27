const globals = @import("globals.zig");
const CellState = globals.CellState;

pub fn generatePuzzle() void {
    globals.board.regions = .{
        .{ 0, 0, 0, 1, 2, 2, 2, 2, 2, 2 },
        .{ 0, 3, 1, 1, 2, 2, 2, 2, 2, 2 },
        .{ 0, 3, 1, 1, 2, 2, 2, 2, 4, 4 },
        .{ 0, 3, 1, 1, 2, 2, 4, 4, 4, 4 },
        .{ 0, 3, 1, 1, 1, 2, 5, 4, 4, 4 },
        .{ 6, 3, 1, 7, 7, 7, 5, 8, 4, 4 },
        .{ 6, 6, 6, 7, 7, 7, 5, 8, 8, 8 },
        .{ 6, 6, 6, 7, 7, 9, 5, 5, 5, 8 },
        .{ 6, 6, 7, 7, 7, 9, 9, 5, 5, 8 },
        .{ 6, 6, 9, 9, 9, 9, 9, 5, 5, 5 },
    };
    globals.board.cellStates = [_][10]CellState{[_]CellState{CellState.EMPTY} ** 10} ** 10;
    for (globals.board.regions, 0..10) |regionsRow, row| {
        for (regionsRow, 0..10) |region, col| globals.board.regionCells[region].pushback = .{ row, col };
    }
}
