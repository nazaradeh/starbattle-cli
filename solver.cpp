#include "grid.hpp"
//#include <algorithm>
//#include <iostream>
//#include <numeric>
#include <vector>

void solvePuzzle() {

std::array<int, 10> markedOffInRow = {0};
std::array<int, 10> markedOffInColumn = {0};
std::array<int, 10> markedOffInRegion = {0};
std::array<int, 30> segmentSizes = {0};
std::array<int, 30> segmentSizesIndices;
std::array<int, 30> segmentOrder;
std::array<std::vector<std::pair<int, int>>, 10> regionCellLocations;
//std::iota(segmentSizesIndices.begin(), segmentSizesIndices.end(), 0);

for (int row = 0; row < 10; ++row) {
	for (int col = 0; col < 10; ++col) {
		regionCellLocations[regions[row][col]].emplace_back(col, row);
		if (cellStates[row][col] == MARKEDOFF) {
			++markedOffInRow[row];
			++markedOffInColumn[col];
			++markedOffInRegion[regions[row][col]];
		}
		else {
			++segmentSizes[row];
			++segmentSizes[10 + col];
			++segmentSizes[20 + regions[row][col]];
		}
	}
}

bool doneTesting = false;
for (auto& cellCoord : regionCellLocations[0]) {
	for (int yOff = -1; yOff < 2; ++yOff) {
		if (cellCoord.second + yOff > -1 && cellCoord.second + yOff < 10) {
			for (int xOff = -1; xOff < 2; ++xOff) {
				if (cellCoord.first + xOff > -1 && cellCoord.first + xOff < 10 && regions[cellCoord.second + yOff][cellCoord.first + xOff] != 0) {
					cellStates[cellCoord.second + yOff][cellCoord.first + xOff] = STAR;
					for (int yOff2 = -1; yOff2 < 2; ++yOff2) {
						if (cellCoord.second + yOff + yOff2 > -1 && cellCoord.second + yOff + yOff2 < 10) {
							for (int xOff2 = -1; xOff2 < 2; ++xOff2) {
								if (cellCoord.first + xOff + xOff2 > -1 && cellCoord.first + xOff + xOff2 < 10 && regions[cellCoord.second + yOff + yOff2][cellCoord.first + xOff + xOff2] == 0) {
									cellStates[cellCoord.second + yOff + yOff2][cellCoord.first + xOff + xOff2] = MARKEDOFF;
									--segmentSizes[20 + 0];
								}
							}
						}
					}
					for (auto& cellCoord2 : regionCellLocations[0]) {
						cellStates[cellCoord2.second][cellCoord2.first] = STAR;
						for (int yOff3 = -1; yOff3 < 2; ++yOff3) {
							if (cellCoord2.second + yOff3 > -1 && cellCoord2.second + yOff3 < 10) {
								for (int xOff3 = -1; xOff3 < 2; ++xOff3) {
									if (cellCoord.first + xOff3 > -1 && cellCoord.first + xOff3 < 10 && regions[cellCoord.second + yOff3][cellCoord.first + xOff3] == 0) {
										cellStates[cellCoord.second + yOff3][cellCoord.first + xOff3] = MARKEDOFF;
										--segmentSizes[20 + 0];
									}
								}
							}
						}
						if (segmentSizes[20 + 0] < 3) {
							cellStates[cellCoord.second + yOff][cellCoord.first + xOff] = MARKEDOFF;
							for (auto& [y, x] : regionCellLocations[0]) {cellStates[y][x] = EMPTY;} // This only works if we started with a blank state
							segmentSizes[20 + 0] = 7; // Only works if we started with a blank state
						}
					}
				}
			}
		}
	}
}

}
