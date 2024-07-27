#ifndef GRID_HPP
#define GRID_HPP
#include <array>

extern std::array<std::array<int, 10>, 10> regions;
enum CellState {EMPTY, MARKEDOFF, STAR};
extern std::array<std::array<CellState, 10>, 10> cellStates;
extern std::array<int, 10> starsInRow;
extern std::array<int, 10> starsInColumn;
extern std::array<int, 10> starsInRegion;

std::wstring buildGrid();

#endif
