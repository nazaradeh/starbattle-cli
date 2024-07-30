#include "grid.hpp"
#include <string>
#include <algorithm>

std::string buildGrid() {

	std::string grid("\x1b[H▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n█"); // Move to top of screen
	//grid.append("▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n█"); // Ceiling row
	//grid.append("\n█"); // Start next line with a wall
	
	for (int row = 0; row < 10; ++row) {

		// Gameplay rows
		for (int col = 0; col < 10; ++col) {

			grid.push_back(' ');
			
			switch (cellStates[row][col]) {
				case EMPTY:
					grid.push_back(' ');
					break;
				case MARKEDOFF:
					grid.append("\x1b[90m·\x1b[37m");
					break;
				case STAR: {
					if (std::ranges::all_of(starsInRow, [](int x) {return x == 2;}) &&
						std::ranges::all_of(starsInColumn, [](int x) {return x == 2;}) &&
						std::ranges::all_of(starsInRegion, [](int x) {return x == 2;})) {
						grid.append("\x1b[32m★\x1b[37m");
						break;
					}
					bool conflictSpotted = starsInRow[row] >= 3 || starsInColumn[col] >= 3 || starsInRegion[regions[row][col]] >= 3;
					//Check adjacent cells for conflicting stars
					for (int xOff = -1; xOff < 2 && !conflictSpotted; ++xOff) {
						for (int yOff = -1; yOff < 2 && !conflictSpotted; ++yOff) {
							if (!(xOff == 0 && yOff == 0) && col+xOff < 10 && row+yOff < 10 && cellStates[row+yOff][col+xOff] == STAR) conflictSpotted = true;
						}
					}
					conflictSpotted ? grid.append("\x1b[31m★\x1b[37m") : grid.append("\x1b[33m★\x1b[37m");
					break;
				}
				default: 
					grid.append("\x1b[91m?");
					break;
			}
			if (col < 9) {
				grid.push_back(' ');
				grid.append(regions[row][col] == regions[row][col+1] ? "│" : "█"); // Divider between two horizontally-adjacent cells
			}
			else {grid.append(" █\n█");} // Right wall
		}

		// Border rows
		if (row < 9) {
			for (int col = 0; col < 9; ++col) {
				
				grid.append(regions[row][col] == regions[row+1][col] ? "───" : "▄▄▄"); // Divider between two verticall-adjacent cells

 				// Cross region: ┼, ▄, or █
				if (regions[row][col] != regions[row][col+1]) grid.append("█"); // The top two cells are in different regions
				else if (regions[row+1][col] == regions[row+1][col+1] && regions[row][col] == regions[row+1][col]) grid.append("┼"); // All in the same region
				else {grid.append("▄");}

			}
			// Final cell
			grid.append(regions[row][9] == regions[row+1][9] ? "───" : "▄▄▄");
			grid.append("█\n█");
		}

		else {
			for (int col = 0; col < 9; ++col) {
				grid.append("▄▄▄"); // Three horizontal lines below a cell
				grid.append(regions[9][col] == regions[9][col+1] ? "▄" : "█");
			}
			grid.append("▄▄▄█\n");
		}
	}

	return grid;
}
