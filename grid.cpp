#include "grid.hpp"
#include <string>
#include <algorithm>

std::wstring buildGrid() {

	std::wstring grid(L"\x1b[H"); // Move to top of screen
	grid.append(41u, L'▄'); // Ceiling row
	grid.append(L"\n█"); // Start next line with a wall
	
	for (int row = 0; row < 10; ++row) {

		// Gameplay rows
		for (int col = 0; col < 10; ++col) {

			grid.push_back(L' ');
			
			switch (cellStates[col][row]) {
				case EMPTY:
					grid.push_back(L' ');
					break;
				case MARKEDOFF:
					grid.append(L"\x1b[90m·\x1b[37m");
					break;
				case STAR: {
					if (std::ranges::all_of(starsInRow, [](int x) {return x == 2;}) &&
						std::ranges::all_of(starsInColumn, [](int x) {return x == 2;}) &&
						std::ranges::all_of(starsInRegion, [](int x) {return x == 2;})) {
						grid.append(L"\x1b[32m★\x1b[37m");
						break;
					}
					bool conflictSpotted = false;
					if (starsInRow[row] >= 3) {conflictSpotted = true;}
					if (starsInColumn[col] >= 3) {conflictSpotted = true;}
					if (starsInRegion[regions[row][col]] >= 3) {conflictSpotted = true;}
					//Check adjacent cells for conflicting stars
					for (int xOff = -1; xOff < 2 && !conflictSpotted; ++xOff) {
						for (int yOff = -1; yOff < 2 && !conflictSpotted; ++yOff) {
							if (!(xOff == 0 && yOff == 0) && col+xOff < 10 && row+yOff < 10 && cellStates[col+xOff][row+yOff] == STAR) {
								conflictSpotted = true;
							}
						}
					}
					conflictSpotted ? grid.append(L"\x1b[31m★\x1b[37m") : grid.append(L"\x1b[33m★\x1b[37m");
					break;
				}
				default: 
					grid.append(L"\x1b[91m?");
					break;
			}
			if (col < 9) {
				grid.push_back(L' ');
				grid.push_back(regions[row][col] == regions[row][col+1] ? L'│' : L'█'); // Divider between two horizontally-adjacent cells
			}
			else {grid.append(L" █\n█");} // Right wall
		}

		// Border rows
		if (row < 9) {
			for (int col = 0; col < 9; ++col) {
				
				grid.append(regions[row][col] == regions[row+1][col] ? L"───" : L"▄▄▄"); // Divider between two verticall-adjacent cells

 				// Cross region: ┼, ▄, or █
				if (regions[row][col] != regions[row][col+1]) {grid.push_back(L'█');} // The top two cells are in different regions
				else if (regions[row+1][col] == regions[row+1][col+1] && regions[row][col] == regions[row+1][col]) {grid.push_back(L'┼');} // All four cells are in the same region
				else {grid.push_back(L'▄');}

			}
			// Final cell
			grid.append(regions[row][9] == regions[row+1][9] ? L"───" : L"▄▄▄");
			grid.append(L"█\n█");
		}

		else {
			for (int col = 0; col < 9; ++col) {
				grid.append(L"▄▄▄"); // Three horizontal lines below a cell
				grid.push_back(regions[9][col] == regions[9][col+1] ? L'▄' : L'█');
			}
			grid.append(L"▄▄▄█\n");
		}
	}

	return grid;
}
