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
std::array<std::vector<std::pair<int, int>>, 10> regionCells;
std::vector<std::pair<int, int>> trackedCells;
const std::array<std::pair<int, int>, 4> ORTHONEIGHBOURS = {{{-1, 0}, {0, -1}, {0, 1}, {1, 0}}};

/*void printLog() {
	std::print("{}\nSize of region 0: {}\nSize of region 3: {}\nBacktracker size: {} \n", buildGrid(), segmentSizes[20+0], segmentSizes[20+3], trackedCells.size());
	std::cin.get();
}*/

std::vector<std::pair<int, int>> getPerimeterCells(const int& region, const std::vector<std::pair<int, int>>& regionCells) {
	std::unordered_set<std::pair<int, int>, std::function<int(const std::pair<int, int>&)>> perimeterSet(0, [](const std::pair<int, int>& pair)
		{return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);});
	for (const auto& cell : regionCells) {
		for (const auto& offset : ORTHONEIGHBOURS) {
			int y = cell.first + offset.first;
			int x = cell.second + offset.second;
			if (y >= 0 && y < 10 && x >= 0 && x < 10 && board.regions[y][x] != region) perimeterSet.emplace(y, x);
		}
	}
	return ranges::to<std::vector>(perimeterSet);
}

void placeStar(const int& region, const std::pair<int, int>& coord) {
	board.cellStates[coord.first][coord.second] = Board::STAR;
	trackedCells.push_back(coord);
	++board.starsInRegion[board.regions[coord.first][coord.second]];
	--segmentSizes[20 + board.regions[coord.first][coord.second]];
	for (const auto& [yOff, xOff] : NEIGHBOURS) {
		if (coord.first + yOff > -1 && coord.first + yOff < 10
			&& coord.second + xOff > -1 && coord.second + xOff < 10
			&& board.regions[coord.first + yOff][coord.second + xOff] == region
			&& board.cellStates[coord.first + yOff][coord.second + xOff] == Board::EMPTY) {
			board.cellStates[coord.first + yOff][coord.second + xOff] = Board::MARKEDOFF;
			trackedCells.push_back({coord.first + yOff, coord.second + xOff});
			--segmentSizes[20 + region];
		}
	}
}

void backtrack(const auto& cells, const std::optional<int>& trackedCellsSizeOld = std::nullopt) {
	for (const auto& [y, x] : cells | views::drop(trackedCellsSizeOld.value_or(0))) {
		if (board.cellStates[y][x] == Board::STAR) --board.starsInRegion[board.regions[y][x]];
		board.cellStates[y][x] = Board::EMPTY;
		++segmentSizes[20 + board.regions[y][x]];
	}
	trackedCells.resize(trackedCellsSizeOld.value_or(0));
}

void markOffCellsInLine(const int region, const auto& regionEmptyCells) {
	const int& regionRow = regionEmptyCells.front().first;
	const int& regionCol = regionEmptyCells.front().second;
	if (ranges::all_of(regionEmptyCells | views::drop(1), [&regionRow](const auto& coord) {return coord.first == regionRow;})) {
		for (const int& col : views::iota(0, 10) | views::filter([&](const int& col) {return board.regions[regionRow][col] != region;}))
			board.cellStates[regionRow][col] = Board::MARKEDOFF;
	} else if (ranges::all_of(regionEmptyCells | views::drop(1), [&regionCol](const auto& coord) {return coord.second == regionCol;})) {
		for (const int& row : views::iota(0, 10) | views::filter([&](const int& row) {return board.regions[row][regionCol] != region;}))
			board.cellStates[row][regionCol] = Board::MARKEDOFF;
	}
}

void perimeterCellCheck(const auto& perimeterCell, const int region, const auto& regionEmptyCells) {
	placeStar(region, {perimeterCell}); // Place star and surround with Board::MARKEDOFF
	// The outside star already causes the region to have too few available cells
	if (segmentSizes[20 + region] < 2 - board.starsInRegion[region]) {
		backtrack(trackedCells);
		board.cellStates[perimeterCell.first][perimeterCell.second] = Board::MARKEDOFF;
		--segmentSizes[20 + board.regions[perimeterCell.first][perimeterCell.second]];
	}
	// The outside star still allows for the region's remaining two stars to be placed
	else if (board.starsInRegion[region] == 0) {
		bool skip = false;
		for (auto& cellCoord2 : regionEmptyCells
			| views::filter([&](const auto& coord) {return board.cellStates[coord.first][coord.second] == Board::EMPTY;})) { // Go through each cell in region 0 again	
			int trackedCellsSizeOld = trackedCells.size();
			placeStar(region, cellCoord2); // Try placing a star to see if another star can fit inside
			if (segmentSizes[20 + region] > 0) { // Another star can fit inside, meaning we cannot rule out the perimeter cell
				backtrack(trackedCells);
				skip = true;
				break;
			} else backtrack(trackedCells, trackedCellsSizeOld); // No other star can fit inside. Keep checking other inner cells.
		}
		if (!skip) { // There is no way to fit both stars in the region, given this perimeter star placement
			backtrack(trackedCells);
			board.cellStates[perimeterCell.first][perimeterCell.second] = Board::MARKEDOFF;
			--segmentSizes[20 + board.regions[perimeterCell.first][perimeterCell.second]];
		}
	} else if (board.starsInRegion[region] == 1) backtrack(trackedCells); // The outside star still allows for the board.regions one remaining star to be placed
}

// The main function
void solvePuzzle() {
	
	// Go through the grid to gather info on each row, column, and region
	for (const int& row : views::iota(0, 10)) {
		for (const int& col : views::iota(0, 10)) {
			regionCells[board.regions[row][col]].emplace_back(row, col);
			if (board.cellStates[row][col] == Board::MARKEDOFF) {
				++markedOffInRow[row];
				++markedOffInColumn[col];
				++markedOffInRegion[board.regions[row][col]];
			} else {
				++segmentSizes[row];
				++segmentSizes[10 + col];
				++segmentSizes[20 + board.regions[row][col]];
			}
		}
	}

	// Check through each row
	for (int row : views::iota(0, 10)) {
		const auto& rowEmptyCells = ranges::to<std::vector>(views::iota(0, 10)
			| views::transform([row](int col) {return std::pair{row, col};})
			| views::filter([&](const auto& coord) {return board.cellStates[coord.first][coord.second] != Board::MARKEDOFF;}));
		int region = board.regions[rowEmptyCells[0].first][rowEmptyCells[1].second];

		// If the only cells in a row not marked off are all in the same region, mark off the parts of said region that is not in this row
		if (ranges::all_of(rowEmptyCells | views::drop(1), [&region](const auto& coord) {return board.regions[coord.first][coord.second] == region;})) {
			for (const auto& coord : regionCells[region] | views::filter([&](auto coord) {return coord.first != row;})) {
				board.cellStates[coord.first][coord.second] = Board::MARKEDOFF;
			}
		} 
		// I might want to do more things with my row of empty cells, here.
	}

	// Check through each region
	for (int region : views::iota(0, 10)) {

		// If this region's cells form a straight line, mark off the rest of the cells on that row or column
		const auto& regionEmptyCells = ranges::to<std::vector>(regionCells[region]
			| views::filter([&](const auto& coord) {return board.cellStates[coord.first][coord.second] != Board::MARKEDOFF;}));

		markOffCellsInLine(region, regionEmptyCells);
		
		for (const std::pair<int, int>& perimeterCell : getPerimeterCells(region, regionEmptyCells)
			| views::filter([&](const auto& cell) {return board.cellStates[cell.first][cell.second] == Board::EMPTY;})) { // Go through the orthogonal perimeter of region
			perimeterCellCheck(perimeterCell, region, regionEmptyCells);	
		}
	}

}
