#ifndef GRID_HPP
#define GRID_HPP
#include <array>

enum CellState {EMPTY, MARKEDOFF, STAR};
extern std::array<std::array<CellState, 10>, 10> cellStates;

// To see if there are too many stars
extern std::array<int, 10> starsInRow;
extern std::array<int, 10> starsInColumn;
extern std::array<int, 10> starsInRegion;

std::wstring buildGrid(const std::array<std::array<int, 10>, 10>& regions);

#endif
