#include "grid.hpp"

void solvePuzzle() {

std::array<int, 10> markedOffInRow = {0};
std::array<int, 10> markedOffInColumn = {0};
std::array<int, 10> markedOffInRegion = {0};
std::array<int, 30> segmentSizes = {0};
std::array<int, 30> segmentOrder;

for (int row = 0; row < 10; ++row) {
	for (int col = 0; col < 10; ++col) {
		if (cellStates[row][col] == MARKEDOFF) {
			++markedOffInRow[row];
			++markedOffInColumn[col];
			++markedOffInRegion[regions[row][col]];
			--segmentSizes[row];
			--segmentSizes[10 + col];
		}
		else ++segmentSizes[20 + regions[row][col]];
	}
}

}
