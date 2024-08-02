#include "globals.hpp"
#include "solver.hpp"
#include <unordered_set>
#include <ranges>
#include <vector>
//#include <print>
#include <functional>
#include <algorithm>

namespace views = std::views;
namespace ranges = std::ranges;
std::array<int, 10> markedOffInRow = {0};
std::array<int, 10> markedOffInColumn = {0};
std::array<int, 10> markedOffInRegion = {0};
std::array<int, 30> segmentSizes = {0};
std::array<std::vector<std::pair<int, int>>, 10> regionCellLocations;
std::vector<std::pair<int, int>> backtracker;
const std::array<std::pair<int, int>, 8> NEIGHBOURS = {{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};
const std::array<std::pair<int, int>, 4> ORTHONEIGHBOURS = {{{-1, 0}, {0, -1}, {0, 1}, {1, 0}}};

/*void printLog() {
	std::print("{}\nSize of region 0: {}\nSize of region 3: {}\nBacktracker size: {} \n", buildGrid(), segmentSizes[20+0], segmentSizes[20+3], backtracker.size());
	std::cin.get();
}*/

std::vector<std::pair<int, int>> getPerimeterCells(const int& region, const std::vector<std::pair<int, int>>& regionCells) {
	std::unordered_set<std::pair<int, int>, std::function<int(const std::pair<int, int>&)>> perimeterSet(0, [](const std::pair<int, int>& pair) {
        	return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
	});
	for (const auto& cell : regionCells) {
		for (const auto& offset : ORTHONEIGHBOURS) {
			int y = cell.first + offset.first;
			int x = cell.second + offset.second;
			if (y >= 0 && y < 10 && x >= 0 && x < 10 && regions[y][x] != region) perimeterSet.emplace(y, x);
		}
	}
	return ranges::to<std::vector>(perimeterSet);
}

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

	

	for (int row : views::iota(0, 10)) {
		for (int col : views::iota(0, 10)) {
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


	for (const int& region : views::iota(0, 10)) {
		
		// If this region's cells form a straight line, mark off the rest of the cells on that row or column
		const auto& regionCoords = ranges::to<std::vector>(regionCellLocations[region] | views::filter([&](const auto& coord) {return cellStates[coord.first][coord.second] != MARKEDOFF;}));
		const int& regionRow = regionCoords.front().first;
		const int& regionCol = regionCoords.front().second;
		if (ranges::all_of(regionCoords | views::drop(1), [&regionRow](const auto& pair) {return pair.first == regionRow;}))
			{for (const int& col : views::iota(0, 10) | views::filter([&](const int& col) {return regions[regionRow][col] != region;})) cellStates[regionRow][col] = MARKEDOFF;}
		else if (ranges::all_of(regionCoords | views::drop(1), [&regionCol](const auto& pair) {return pair.second == regionCol;}))
			{for (const int& row : views::iota(0, 10) | views::filter([&](const int& row) {return regions[row][regionCol] != region;})) cellStates[row][regionCol] = MARKEDOFF;}
		
		for (const std::pair<int, int>& perimeterCell : getPerimeterCells(region, regionCoords)) { // Go through the orthogonal perimeter of region
			if (cellStates[perimeterCell.first][perimeterCell.second] == EMPTY) {
				placeStar(region, {perimeterCell}); // Place star and surround with MARKEDOFF
	 
				// The outside star already causes the region to have too few available cells
				if (segmentSizes[20 + region] < 2 - starsInRegion[region]) {
					for (auto& [y, x] : backtracker) {
						if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
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
					for (auto& cellCoord2 : regionCoords) { // Go through each cell in region 0 again	
						if (cellStates[cellCoord2.first][cellCoord2.second] == EMPTY) {
							int backtrackerSizeOld = backtracker.size();
							placeStar(region, cellCoord2); // Try placing a star to see if another star can fit inside
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
								for (auto& [y, x] : backtracker | views::drop(backtrackerSizeOld)) {
									if (cellStates[y][x] == STAR) --starsInRegion[regions[y][x]];
									cellStates[y][x] = EMPTY;
									++segmentSizes[20 + regions[y][x]];
								}
								backtracker.resize(backtrackerSizeOld);
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

				// The outside star still allows for the regions one remaining star to be placed
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
