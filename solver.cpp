#include "grid.hpp"
//#include <algorithm>
#include <ranges>
#include <iostream>
//#include <numeric>
#include <vector>
#include <Windows.h>
#include <print>

std::array<int, 10> markedOffInRow = {0};
std::array<int, 10> markedOffInColumn = {0};
std::array<int, 10> markedOffInRegion = {0};
std::array<int, 30> segmentSizes = {0};
std::array<int, 30> segmentSizesIndices;
std::array<int, 30> segmentOrder;
std::array<std::vector<std::pair<int, int>>, 10> regionCellLocations;
std::vector<std::pair<int, int>> backtracker;
//std::iota(segmentSizesIndices.begin(), segmentSizesIndices.end(), 0);

void placeStar(int region, std::pair<int, int> coord) {
	cellStates[coord.first][coord.second] = STAR;
	backtracker.push_back({coord.first, coord.second});
	++starsInRegion[regions[coord.first][coord.second]];
	if (region == regions[coord.first][coord.second]) {--segmentSizes[20 + regions[coord.first][coord.second]];}
	for (int yOff : {-1, 0, 1}) {
		for (int xOff : {-1, 0, 1}) {
			if (coord.first + yOff > -1 &&
					coord.first + yOff < 10 &&
					coord.second + xOff > -1 &&
					coord.second + xOff < 10 &&
					regions[coord.first + yOff][coord.second + xOff] == region &&
					cellStates[coord.first + yOff][coord.second + xOff] == EMPTY) {
				cellStates[coord.first + yOff][coord.second + xOff] = MARKEDOFF;
				backtracker.push_back({coord.first + yOff, coord.second + xOff});
				--segmentSizes[20 + region];
			}
		}
	}
	
}

void printLog() {
	std::print("{}Segment size: {}\nStars in region: {}\nBacktracker size: {}\n", buildGrid(), segmentSizes[20+3], starsInRegion[3], backtracker.size());
	//std::ranges::for_each(backtracker, [](const auto& pair) { std::print("{},{} ", pair.first, pair.second); });
	//Sleep(2000);
	std::cin.get();
}

void solvePuzzle() {

for (int row = 0; row < 10; ++row) {
	for (int col = 0; col < 10; ++col) {
		regionCellLocations[regions[row][col]].emplace_back(row, col);
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

int region = 0;
for (auto& cellCoord : regionCellLocations[region]) { // Go through each cell in region 0
	for (int yOff = -1; yOff < 2; ++yOff) { // Go through each cell surrounding region 0
		for (int xOff = -1; xOff < 2; ++xOff) {
			if (cellCoord.first + yOff > -1 && // Make sure it's not out of bounds, it is outside of region 0, and is EMPTY
					cellCoord.first + yOff < 10 &&
					cellCoord.second + xOff > -1 &&
					cellCoord.second + xOff < 10 &&
					regions[cellCoord.first + yOff][cellCoord.second + xOff] != region &&
					cellStates[cellCoord.first + yOff][cellCoord.second + xOff] == EMPTY) {
				placeStar(region, {cellCoord.first + yOff, cellCoord.second + xOff}); // Place star and surround with MARKEDOFF
				//printLog();
				if (segmentSizes[20 + region] < 2 - starsInRegion[region]) {
					//std::print("case 1");
					for (auto& [y, x] : backtracker) {cellStates[y][x] = EMPTY;}
							segmentSizes[20 + region] += backtracker.size() - 1;
							backtracker.clear();
							//printLog();
							cellStates[cellCoord.first + yOff][cellCoord.second + xOff] = MARKEDOFF; // The original star we placed cannot be a star
							//printLog();
					
				}
				else if (starsInRegion[region] == 0) {
					//std::print("case 2");
					bool skip = false;
					for (auto& cellCoord2 : regionCellLocations[region]) { // Go through each cell in region 0 again	
						if (cellStates[cellCoord2.first][cellCoord2.second] == EMPTY) {
							int backtrackerSizeOld = backtracker.size();
							int backtrackerSizeNew = 0;
							placeStar(region, cellCoord2); // Try placing a star to see if another star can fit inside
							backtrackerSizeNew = backtracker.size() - backtrackerSizeOld;
							//printLog();
							if (segmentSizes[20 + region] > 0) {
								for (auto& [y, x] : backtracker) {
									if (cellStates[y][x] == STAR) {--starsInRegion[regions[y][x]];}
									cellStates[y][x] = EMPTY;
									//if (std::ranges::any_of(regionCellLocations[region], [y, x](const auto& p) {return p == std::pair(y, x); })) {}
									//printLog();
								}
								segmentSizes[20 + region] += backtracker.size() - 1;
								backtracker.clear();
								//printLog();
								skip = true;
								break;
							}
							else {
								for (auto& [y, x] : backtracker | std::views::drop(backtracker.size() - backtrackerSizeNew)) {
									if (cellStates[y][x] == STAR) {--starsInRegion[regions[y][x]];}
									cellStates[y][x] = EMPTY;
									//printLog();
								}
								segmentSizes[20 + region] += backtrackerSizeNew;
								backtracker.resize(backtracker.size() - backtrackerSizeNew);
								//printLog();
							}
						}	
					}
					if (!skip) {
						for (auto& [y, x] : backtracker) {
							if (cellStates[y][x] == STAR) {--starsInRegion[regions[y][x]];}
							cellStates[y][x] = EMPTY;
						}
						segmentSizes[20 + region] += backtracker.size() - 1;
						backtracker.clear();
						cellStates[cellCoord.first + yOff][cellCoord.second + xOff] = MARKEDOFF; // The original star we placed cannot be a star
						//printLog();
					}
				}
				else if (starsInRegion[region] == 1) {
		
					//std::print("case 3");
					//printLog();
					for (auto& [y, x] : backtracker) {
						if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
						cellStates[y][x] = EMPTY;
						}
						segmentSizes[20 + region] += backtracker.size() - 1;
						backtracker.clear();
						//printLog();
				}
			}
		}
	}
}


//auto& cellCoord = regionCellLocations[0][0];

/*
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
*/
}
