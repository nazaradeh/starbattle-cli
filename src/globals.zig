pub const Point = struct { row: u8, col: u8 };

pub const OCTONEIGHBOURS: [8]Point = .{
    .{ -1, -1 },
    .{ -1, 0 },
    .{ -1, 1 },
    .{ 0, -1 },
    .{ 0, 1 },
    .{ 1, -1 },
    .{ 1, 0 },
    .{ 1, 1 },
};

pub const ORTHONEIGHBOURS: [4]Point = .{
    .{ -1, 0 },
    .{ 0, -1 },
    .{ 0, 1 },
    .{ 1, 0 },
};

pub const CellState = enum { EMPTY, MARKEDOFF, STAR };

const Board = struct {
    regions: [10][10]u8,
    cellStates: [10][10]CellState,
    regionCells: [10][]Point,
    //starsInRow: [10]u8,
    //starsInColumn: [10]u8,
    //starsInRegion: [10]u8,
    //markedOffInRow: [10]u8,
    //markedOffInColumn: [10]u8,
    //markedOffInRegion: [10]u8,
};

pub var board: Board = undefined;
