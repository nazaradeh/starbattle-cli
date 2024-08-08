#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <array>

extern std::array<std::array<int, 10>, 10> regions;
enum CellState {EMPTY, MARKEDOFF, STAR};
extern std::array<std::array<CellState, 10>, 10> cellStates;
extern std::array<int, 10> starsInRow;
extern std::array<int, 10> starsInColumn;
extern std::array<int, 10> starsInRegion;
const std::array<std::pair<int, int>, 8> NEIGHBOURS = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

class Board {
public:
	Board();
	std::array<std::array<int, 10>, 10> regions;
	void toggleCellState();
	void printGrid() const;

private:
	std::array<std::array<CellState, 10>, 10> cellStates;
	std::array<int, 10> starsInRow;
	std::array<int, 10> starsInColumn;
	std::array<int, 10> starsInRegion;
};

extern Board board;

#endif
