#include "grid.hpp"
#include <string>
#include <array>

std::wstring buildGrid(const std::array<std::array<int, 10>, 10>& regions) {
	
	std::wstring grid(41u, L'▄'); // Ceiling row
	grid.append(L"\n█");
	
	for (int row = 0; row < 10; ++row) {

		// Gameplay rows
		for (int col = 0; col < 9; ++col) {
			grid.append(L"   "); // Three spaces creating an empty cell
			grid.push_back(regions[row][col] == regions[row][col+1] ? L'│' : L'█'); // Divider between two horizontally-adjacent cells
		}
		grid.append(L"   █\n█");

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
				grid.append(L"▄▄▄"); // Three horizontal lines below a cell: ▄▄▄
				grid.push_back(regions[9][col] == regions[9][col+1] ? L'▄' : L'█');
			}
			grid.append(L"▄▄▄█\n");
		}
	}

	return grid;
}
