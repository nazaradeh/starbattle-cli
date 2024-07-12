#include "grid.h"
#include <iostream>
#include <windows.h>

wchar_t horizontalBar(bool b) {return b ? L'─' : L'═';}

wchar_t verticalBar(bool b) {return b ? L'│' : L'║';}

wchar_t leftTWall(bool b) {return b ? L'╟' : L'╠';}

wchar_t rightTWall(bool b) {return b ? L'╢' : L'╣';}

wchar_t TCeiling(bool b) {return b ? L'╤' : L'╦';}

wchar_t TFloor(bool b) {return b ? L'╧' : L'╩';}

wchar_t cross(bool b[4]) {
	char crossComparisons = b[0] << 3 | b[1] << 2 | b[2] << 1 | b[3];
	switch(crossComparisons) {
		case 0b0000: return L'╬';
		case 0b0001: return L'╣';
		case 0b0010: return L'╠';
		case 0b0011: return L'╫';
		case 0b0100: return L'╩';
		case 0b0101: return L'╝';
		case 0b0110: return L'╚';
		case 0b0111: return L'?'; // Not possible.
		case 0b1000: return L'╦';
		case 0b1001: return L'╗';
		case 0b1010: return L'╔';
		case 0b1011: return L'?'; // Not possible.
		case 0b1100: return L'╪';
		case 0b1101: return L'?'; // Not possible.
		case 0b1110: return L'?'; // Not possible.
		case 0b1111: return L'┼';
		default: return L'?'; // Not possible.
	}
}

void drawGrid(int regions[10][10]) {
	
	// The grid itself
	wchar_t grid[21][41];
	
	// Ceiling row
	grid[0][0] = L'╔'; // Top left corner
	for (int i = 1; i < 40; i += 4) {
		for (int j = 0; j < 3; ++j) {grid[0][i+j] = L'═';} // Three horizontal lines: ═══
		if (i+3 < 39) {grid[0][i+3] = TCeiling(regions[0][(i-1)/4] == regions[0][(i+3)/4]);} // ╤ or ╦. Increments when i is at 4, 8, 12, etc.
		else {grid[0][40] = L'╗';} // Top right corner
	}
	
	for (int h = 1; h < 20; h += 2) {
		// Conversion from grid rows to gameplay rows = (h-1)/2
		// Conversion from grid columns to gameplay columns = (i-1)/4
	
		// Gameplay rows
		grid[h][0] = L'║';
		for (int i = 1; i < 40; i += 4) {
			for (int j = 0; j < 3; ++j) {grid[h][i+j] = L' ';}
			if (i+3 < 39) {grid[h][i+3] = verticalBar(regions[(h-1)/2][(i-1)/4] == regions[(h-1)/2][(i+3)/4]);} // │ or ║
			else {grid[h][40] = L'║';}
		}

		// Border rows
		if (h < 19) {
			grid[h+1][0] = leftTWall(regions[(h-1)/2][0] == regions[(h+1)/2][0]); // ╟ or ╠
			for (int i = 1; i < 40; i += 4) {
				for (int j = 0; j < 3; ++j) {grid[h+1][i+j] = horizontalBar(regions[(h-1)/2][(i-1)/4] == regions[(h+1)/2][(i-1)/4]);} // Three horizontal lines: ─── or ═══
				if (i+3 < 39) {grid[h+1][i+3] = cross((bool[4])
						{regions[(h-1)/2][(i-1)/4] == regions[(h-1)/2][(i+3)/4],
						regions[(h+1)/2][(i-1)/4] == regions[(h+1)/2][(i+3)/4],
						regions[(h-1)/2][(i-1)/4] == regions[(h+1)/2][(i-1)/4],
						regions[(h-1)/2][(i+3)/4] == regions[(h+1)/2][(i+3)/4]});} // ┼, ╬, etc.
				else {grid[h+1][40] = rightTWall(regions[(h-1)/2][9] == regions[(h+1)/2][9]);} // ╢ or ╣
			}
		}

		// Floor row
		else {
			grid[20][0] = L'╚'; // Bottom left corner
			for (int i = 1; i < 40; i += 4) {
				for (int j = 0; j < 3; ++j) {grid[20][i+j] = L'═';} // Three horizontal lines: ═══
				if (i+3 < 39) {grid[20][i+3] = TFloor(regions[9][(i-1)/4] == regions[9][(i+3)/4]);} // ╧ or ╩
				else {grid[20][40] = L'╝';} // Bottom right corner
			}
		}

	}

	// Required for displaying box drawing characters.
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));

	std::cout << "\x1b[?1049h"; // Switch to the alternate screen buffer.
	std::cout << "\x1b[H"; // Move to the top of the screen.	
	// Display grid
	for (wchar_t (&row)[41] : grid) {
		for (wchar_t col : row) {std::wcout << col;}
		std::wcout << std::endl;
	}

}
