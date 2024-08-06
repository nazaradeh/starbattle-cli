#include "globals.hpp"
#include "grid.hpp"
#include <algorithm>
#include <ranges>

std::string cellElement(int row, int col) {
	switch (cellStates[row][col]) {
		case EMPTY:
			return "   ";
		case MARKEDOFF:
			return " \x1b[90m·\x1b[39m "; // Grey
		case STAR: 
			if (std::ranges::all_of(starsInRow, [](int n) {return n == 2;}) &&
				std::ranges::all_of(starsInColumn, [](int n) {return n == 2;}) &&
				std::ranges::all_of(starsInRegion, [](int n) {return n == 2;})) {
				return " \x1b[32m★\x1b[39m "; // Green
			}
			if (starsInRow[row] >= 3 || starsInColumn[col] >= 3 || starsInRegion[regions[row][col]] >= 3)
				return " \x1b[31m★\x1b[39m "; // Red
			for (const auto& [yOff, xOff] : NEIGHBOURS) if (col+xOff < 10 && row+yOff < 10 && cellStates[row+yOff][col+xOff] == STAR)
				return " \x1b[31m★\x1b[39m "; // Red
			return " \x1b[33m★\x1b[39m "; // Yellow
		default:
			return " ? "; // Should never happen
	}
}

void cellUnderside(std::string& grid, int row) { 
	for (const int col : std::views::iota(0,9)) {
		// Top two cells in different regions (█), all cells in the same region (┼), otherwise (▄)
		if (regions[row][col] != regions[row][col+1]) {
			if (regions[row][col] == regions[row+1][col]) grid.append("───█");
			else grid.append("▄▄▄█");
		} else if (regions[row+1][col] == regions[row+1][col+1] && regions[row][col] == regions[row+1][col]) {
			grid.append("───┼");
		} else {
			if (regions[row][col] == regions[row+1][col]) grid.append("───▄");
			else grid.append("▄▄▄▄");
		}
	}
	// Final cell
	if (regions[row][9] == regions[row+1][9]) grid.append("───█\n█");
	else grid.append("▄▄▄█\n█");
}

// The main function
std::string buildGrid() {
	std::string grid("\x1b[H\x1b[39m▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n█"); // Move to top of screen and print ceiling
	for (int row : std::views::iota(0,10)) {
		// Gameplay rows
		for (int col : std::views::iota(0,10)) {
			grid.append(cellElement(row, col)); // Inside the cell itself
			if (col == 9) grid.append("█\n█"); // Right wall
			else if (regions[row][col] == regions[row][col+1]) grid.append("│"); // Divider within the same region
			else grid.append("█"); // Divider between two regions
		}
		// Border rows
		if (row < 9) cellUnderside(grid, row); // Border row between two vertically-adjacent gameplay rows
	}
	// Floor row
	for (const int col : std::views::iota(0,9)) {
		if (regions[9][col] == regions[9][col+1]) grid.append("▄▄▄▄");
		else grid.append("▄▄▄█");
	}
	grid.append("▄▄▄█\n");
	return grid;
}
