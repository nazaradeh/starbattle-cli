#include "grid.hpp"
//#include <algorithm>
//#include <utility>
#include <unordered_set>
#include <ranges>
//#include <iostream>
//#include <numeric>
#include <vector>
//#include <print>

std::array<int, 10> markedOffInRow = {0};
std::array<int, 10> markedOffInColumn = {0};
std::array<int, 10> markedOffInRegion = {0};
std::array<int, 30> segmentSizes = {0};
//std::array<int, 30> segmentSizesIndices;
//std::array<int, 30> segmentOrder;
std::array<std::vector<std::pair<int, int>>, 10> regionCellLocations;
std::vector<std::pair<int, int>> backtracker;

const std::array<std::pair<int, int>, 8> NEIGHBOURS = {{
	{-1, -1}, {-1, 0}, {-1, 1},
	{0, -1},           {0, 1},
	{1, -1},  {1, 0},  {1, 1}
}};

const std::array<std::pair<int, int>, 8> ORTHONEIGHBOURS = {{{-1, 0}, {0, -1}, {0, 1}, {1, 0}}};

struct pair_hash {
	std::size_t operator() (const std::pair<int, int>& pair) const {
		return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
	}
};

std::vector<std::pair<int, int>> getPerimeterCells(const int& region, const std::vector<std::pair<int, int>>& regionCells) {
	std::unordered_set<std::pair<int, int>, pair_hash> perimeterSet;
	for (const auto& cell : regionCells) {
		for (const auto& offset : ORTHONEIGHBOURS) {
			int y = cell.first + offset.first;
			int x = cell.second + offset.second;
			if (y >= 0 && y < 10 && x >= 0 && x < 10 && regions[y][x] != region) perimeterSet.emplace(y, x);
		}
	}
	return std::ranges::to<std::vector>(perimeterSet);
}

/*void printLog() {
	std::print("{}\nSize of region 0: {}\nSize of region 3: {}\nBacktracker size: {} \n", buildGrid(), segmentSizes[20+0], segmentSizes[20+3], backtracker.size());
	std::cin.get();
}*/

void placeStar(const int& region, const std::pair<int, int>& coord) {
	cellStates[coord.first][coord.second] = STAR;
	backtracker.push_back(coord);
	++starsInRegion[regions[coord.first][coord.second]];
	--segmentSizes[20 + regions[coord.first][coord.second]];
	for (const auto& [yOff, xOff] : NEIGHBOURS) {
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


	for (int region : std::views::iota(0, 10)) {
		
		for (auto& perimeterCell : getPerimeterCells(region, regionCellLocations[region])) { // Go through the orthogonal perimeter of region
			if (cellStates[perimeterCell.first][perimeterCell.second] == EMPTY) {
				placeStar(region, {perimeterCell}); // Place star and surround with MARKEDOFF
	 
				// The outside star already causes the region to have too few available cells
				if (segmentSizes[20 + region] < 2 - starsInRegion[region]) {
					for (auto& [y, x] : backtracker) {
						cellStates[y][x] = EMPTY;
						++segmentSizes[20 + regions[y][x]];
					}
					backtracker.clear();
					cellStates[perimeterCell.first][perimeterCell.second] = MARKEDOFF;
					--segmentSizes[20 + regions[perimeterCell.first][perimeterCell.second]];
				}

				// The outside star still allows for the region's remaining two stars to be placed
				else if (starsInRegion[region] == 0) {
					bool skip = false;
					for (auto& cellCoord2 : regionCellLocations[region]) { // Go through each cell in region 0 again	
						if (cellStates[cellCoord2.first][cellCoord2.second] == EMPTY) {
							int backtrackerSizeOld = backtracker.size();
							int backtrackerSizeNew = 0;
							placeStar(region, cellCoord2); // Try placing a star to see if another star can fit inside
							backtrackerSizeNew = backtracker.size() - backtrackerSizeOld;
							if (segmentSizes[20 + region] > 0) {
								for (auto& [y, x] : backtracker) {
									if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
									cellStates[y][x] = EMPTY;
									++segmentSizes[20 + regions[y][x]];
								}
								backtracker.clear();
								skip = true;
								break;
							}
							else {
								for (auto& [y, x] : backtracker | std::views::drop(backtracker.size() - backtrackerSizeNew)) {
									if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
									cellStates[y][x] = EMPTY;
									++segmentSizes[20 + regions[y][x]];
								}
								backtracker.resize(backtracker.size() - backtrackerSizeNew);
							}
						}	
					}
					if (!skip) {
						for (auto& [y, x] : backtracker) {
							if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
							cellStates[y][x] = EMPTY;
							++segmentSizes[20 + regions[y][x]];
						}
						backtracker.clear();
						cellStates[perimeterCell.first][perimeterCell.second] = MARKEDOFF; // The original star we placed cannot be a star
						--segmentSizes[20 + regions[perimeterCell.first][perimeterCell.second]];
					}
				}

				else if (starsInRegion[region] == 1) {
					for (auto& [y, x] : backtracker) {
						if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
						cellStates[y][x] = EMPTY;
						++segmentSizes[20 + regions[y][x]];
						}
					backtracker.clear();
				}
			}
		}
	}

}
