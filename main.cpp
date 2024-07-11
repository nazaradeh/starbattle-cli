#include <iostream>
#include <windows.h>
#include <algorithm>

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


int main() {

	// Required for displaying box drawing characters.
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));

	std::cout << "\x1b[?1049h"; // Switch to the alternate screen buffer.
	std::cout << "\x1b[H"; // Move to the top of the screen.

	// Each region is numbered from 0 to 9. Each region must contain exactly 2 stars.
	int regions[10][10] = {
		{0, 0, 0, 1, 2, 2, 2, 2, 2, 2},
		{0, 3, 1, 1, 2, 2, 2, 2, 2, 2},
		{0, 3, 1, 1, 2, 2, 2, 2, 4, 4},
		{0, 3, 1, 1, 2, 2, 4, 4, 4, 4},
		{0, 3, 1, 1, 1, 2, 5, 4, 4, 4},
		{6, 3, 1, 7, 7, 7, 5, 8, 4, 4},
		{6, 6, 6, 7, 7, 7, 5, 8, 8, 8},
		{6, 6, 6, 7, 7, 9, 5, 5, 5, 8},
		{6, 6, 7, 7, 7, 9, 9, 5, 5, 8},
		{6, 6, 9, 9, 9, 9, 9, 5, 5, 5}
	};


	wchar_t starsInput[10][10];
	for (wchar_t (&row)[10] : starsInput) {std::fill(std::begin(row), std::end(row), L' ');}


	int cursorLocation[2] = {2, 1};


	bool verticalComparisons[9][10];
	for (int row = 0; row < 9; ++row) {
    		for (int col = 0; col < 10; ++col) {
			verticalComparisons[row][col] = regions[row][col] == regions[row+1][col];
		}
	}	

	bool horizontalComparisons[10][9];
	for (int row = 0; row < 10; ++row) {
    		for (int col = 0; col < 9; ++col) {
			horizontalComparisons[row][col] = regions[row][col] == regions[row][col+1];
		}
	}
	
	
	wchar_t grid[21][41];	
	
	// Ceiling row
	grid[0][0] = L'╔'; // Top left corner
	for (int i = 1; i < 40; i += 4) {
		for (int j = 0; j < 3; ++j) {grid[0][i+j] = L'═';} // Three horizontal lines: ═══
		if (i+3 < 39) {grid[0][i+3] = TCeiling(horizontalComparisons[0][(i-1)/4]);} // ╤ or ╦. Increments when i is at 4, 8, 12, etc.
		else {grid[0][40] = L'╗';} // Top right corner
	}
	
	for (int h = 1; h < 20; h += 2) {
		// Conversion from grid rows to gameplay rows = (h-1)/2
		// Conversion from grid columns to gameplay columns = (i-1)/4
	
		// Gameplay rows
		grid[h][0] = L'║';
		for (int i = 1; i < 40; i += 4) {
			grid[h][i] = L' ';
			grid[h][i+1] = starsInput[(h-1)/2][(i-1)/4]; // Play area, where a star can be added. Increments when i is at 2, 6, 10, etc.
			grid[h][i+2] = L' ';
			if (i+3 < 39) {grid[h][i+3] = verticalBar(horizontalComparisons[(h-1)/2][(i-1)/4]);} // │ or ║
			else {grid[h][40] = L'║';}
		}

		// Border rows
		if (h < 19) {
			grid[h+1][0] = leftTWall(verticalComparisons[(h-1)/2][0]); // ╟ or ╠
			for (int i = 1; i < 40; i += 4) {
				for (int j = 0; j < 3; ++j) {grid[h+1][i+j] = horizontalBar(verticalComparisons[(h-1)/2][(i-1)/4]);} // Three horizontal lines: ─── or ═══
				if (i+3 < 39) {grid[h+1][i+3] = cross((bool[4])
						{horizontalComparisons[(h-1)/2][(i-1)/4], horizontalComparisons[(h+1)/2][(i-1)/4],
						verticalComparisons[(h-1)/2][(i-1)/4], verticalComparisons[(h-1)/2][(i+3)/4]});} // ┼, ╬, etc.
				else {grid[h+1][40] = rightTWall(verticalComparisons[(h-1)/2][9]);} // ╢ or ╣
			}
		}

		// Floor row
		else {
			grid[20][0] = L'╚'; // Bottom left corner
			for (int i = 1; i < 40; i += 4) {
				for (int j = 0; j < 3; ++j) {grid[20][i+j] = L'═';} // Three horizontal lines: ═══
				if (i+3 < 39) {grid[20][i+3] = TFloor(horizontalComparisons[9][(i-1)/4]);} // ╧ or ╩
				else {grid[20][40] = L'╝';} // Bottom right corner
			}
		}

	}

	
	// Display grid
	for (wchar_t (&row)[41] : grid) {
		for (wchar_t col : row) {std::wcout << col;}
		std::wcout << std::endl;
	}

    	// Cursor position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	// Create a COORD struct to hold the cursor position
    	COORD position;
	position.X = cursorLocation[0];  // X coordinate (0-based index)
	position.Y = cursorLocation[1];  // Y coordinate (0-based index)
    	// Set the console cursor position
    	SetConsoleCursorPosition(hStdout, position);

	// Controls
	while (true) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::cout << "\x1b[?1049l";
			break;
		}

		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('H')) && cursorLocation[0] > 2) {
			cursorLocation[0] -= 4;
			position.X = cursorLocation[0];
    			SetConsoleCursorPosition(hStdout, position);	
			while ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('H') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_DOWN) ||GetAsyncKeyState('S') ||  GetAsyncKeyState('J')) && cursorLocation[1] < 19) {
			cursorLocation[1] += 2;
			position.Y = cursorLocation[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('J') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('K')) && cursorLocation[1] > 1) {
			cursorLocation[1] -= 2;
			position.Y = cursorLocation[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('K') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('L')) && cursorLocation[0] < 38) {
			cursorLocation[0] += 4;
			position.X = cursorLocation[0];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('L') & 0x8000));
		}

	}
	return 0;
}
