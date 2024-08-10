#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <array>
/*
extern std::array<std::array<int8_t, 10>, 10> regions;
enum CellState {EMPTY, MARKEDOFF, STAR};
extern std::array<std::array<CellState, 10>, 10> cellStates;
extern std::array<int8_t, 10> starsInRow;
extern std::array<int8_t, 10> starsInColumn;
extern std::array<int8_t, 10> starsInRegion;
*/
const std::array<std::pair<int8_t, int8_t>, 8> NEIGHBOURS = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

class Board {
public:
	//Board();
	std::array<std::array<int8_t, 10>, 10> regions;
	void toggleCellState();
	void printGrid() const;
	enum CellState {EMPTY, MARKEDOFF, STAR};
	std::array<std::array<CellState, 10>, 10> cellStates;
	std::array<int8_t, 10> starsInRow;
	std::array<int8_t, 10> starsInColumn;
	std::array<int8_t, 10> starsInRegion;
};

extern Board board;

#endif
