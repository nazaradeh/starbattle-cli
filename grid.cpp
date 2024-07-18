#include "grid.h"
#include <iostream>
#include <windows.h>
#include <array>

wchar_t horizontalBar(bool b) {return b ? L'─' : L'▄';}
wchar_t verticalBar(bool b) {return b ? L'│' : L'█';}
wchar_t TFloor(bool b) {return b ? L'▄' : L'█';}

wchar_t cross(std::array<bool, 4> b) {
	if (!b[0]) {return L'█';} // The top two cells are in different regions
	else if (b[0] && b[1] && b[2] && b[3]) {return L'┼';} // All four surrounding cells are in the same region
	else {return L'▄';}
}

void drawGrid(const std::array<std::array<int, 10>, 10>& regions) {
	// The grid itself
	std::array<std::array<wchar_t, 41>, 21> grid;
	
	// Ceiling row
	for (auto& col : grid[0]) {col = L'▄';}

	for (int row = 1; row < 20; row += 2) {
		// Conversion from grid rows to gameplay rows = (row-1)/2
		// Conversion from grid columns to gameplay columns = (col-1)/4
	
		// Gameplay rows
		grid[row][0] = L'█';
		for (int col = 1; col < 40; col += 4) {
			for (int j = 0; j < 3; ++j) {grid[row][col+j] = L' ';} // Three spaces creating an empty cell
			if (col+3 < 39) {grid[row][col+3] = verticalBar(regions[(row-1)/2][(col-1)/4] == regions[(row-1)/2][(col+3)/4]);} // │ or █
			else {grid[row][40] = L'█';}
		}

		// Border rows
		if (row < 19) {
			grid[row+1][0] = L'█';
			for (int col = 1; col < 40; col += 4) {
				for (int j = 0; j < 3; ++j) {grid[row+1][col+j] = horizontalBar(regions[(row-1)/2][(col-1)/4] == regions[(row+1)/2][(col-1)/4]);} // Three horizontal lines: ─── or ▄▄▄
				if (col+3 < 39) {grid[row+1][col+3] = cross((std::array<bool, 4>)
						{regions[(row-1)/2][(col-1)/4] == regions[(row-1)/2][(col+3)/4],
						regions[(row+1)/2][(col-1)/4] == regions[(row+1)/2][(col+3)/4],
						regions[(row-1)/2][(col-1)/4] == regions[(row+1)/2][(col-1)/4],
						regions[(row-1)/2][(col+3)/4] == regions[(row+1)/2][(col+3)/4]});} // ┼, ▄, or █
				else {grid[row+1][40] = L'█';}
			}
		}

		// Floor row
		else {
			grid[20][0] = L'█'; // Bottom left corner
			for (int col = 1; col < 41; col += 4) {
				for (int i = 0; i < 3; ++i) {grid[20][col+i] = L'▄';} // Three horizontal lines: ▄▄▄
				grid[20][col+3] = TFloor(regions[9][(col-1)/4] == regions[9][(col+3)/4]); // ▄, or █
			}
		}

	}

	std::wstring result;

	for (auto &row : grid) {
		for (auto &col : row) {result.push_back(col);}
		result.push_back(L'\n');
	}

	// Required for displaying box drawing characters.
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));

	std::wcout << "\x1b[?1049h" // Switch to the alternate screen buffer.
		<< "\x1b[H"; // Move to the top of the screen.	
	std::wcout << result;

}
