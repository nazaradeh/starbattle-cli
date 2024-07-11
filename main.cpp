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

	
	wchar_t grid[21][41] = {

		{L'╔', L'═', L'═', L'═', TCeiling(horizontalComparisons[0][0]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][1]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][2]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][3]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][4]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][5]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][6]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][7]),
			L'═', L'═', L'═', TCeiling(horizontalComparisons[0][8]),
			L'═', L'═', L'═', L'╗'},

		{L'║', L' ', starsInput[0][0], L' ', verticalBar(horizontalComparisons[0][0]),
			L' ', starsInput[0][1], L' ', verticalBar(horizontalComparisons[0][1]),
			L' ', starsInput[0][2], L' ', verticalBar(horizontalComparisons[0][2]),
			L' ', starsInput[0][3], L' ', verticalBar(horizontalComparisons[0][3]),
			L' ', starsInput[0][4], L' ', verticalBar(horizontalComparisons[0][4]),
			L' ', starsInput[0][5], L' ', verticalBar(horizontalComparisons[0][5]),
			L' ', starsInput[0][6], L' ', verticalBar(horizontalComparisons[0][6]),
			L' ', starsInput[0][7], L' ', verticalBar(horizontalComparisons[0][7]),
			L' ', starsInput[0][8], L' ', verticalBar(horizontalComparisons[0][8]),
			L' ', starsInput[0][9], L' ', L'║'},

		{leftTWall(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			cross((bool[4]){horizontalComparisons[0][0], horizontalComparisons[1][0], verticalComparisons[0][0], verticalComparisons[0][1]}),
			horizontalBar(verticalComparisons[0][1]),
			horizontalBar(verticalComparisons[0][1]),
			horizontalBar(verticalComparisons[0][1]),
			cross((bool[4]){horizontalComparisons[0][1], horizontalComparisons[1][1], verticalComparisons[0][1], verticalComparisons[0][2]}),
			horizontalBar(verticalComparisons[0][2]),
			horizontalBar(verticalComparisons[0][2]),
			horizontalBar(verticalComparisons[0][2]),
			cross((bool[4]){horizontalComparisons[0][2], horizontalComparisons[1][2], verticalComparisons[0][2], verticalComparisons[0][3]}),
			horizontalBar(verticalComparisons[0][3]),
			horizontalBar(verticalComparisons[0][3]),
			horizontalBar(verticalComparisons[0][3]),
			cross((bool[4]){horizontalComparisons[0][3], horizontalComparisons[1][3], verticalComparisons[0][3], verticalComparisons[0][4]}),
			horizontalBar(verticalComparisons[0][4]),
			horizontalBar(verticalComparisons[0][4]),
			horizontalBar(verticalComparisons[0][4]),
			cross((bool[4]){horizontalComparisons[0][4], horizontalComparisons[1][4], verticalComparisons[0][4], verticalComparisons[0][5]}),
			horizontalBar(verticalComparisons[0][5]),
			horizontalBar(verticalComparisons[0][5]),
			horizontalBar(verticalComparisons[0][5]),
			cross((bool[4]){horizontalComparisons[0][5], horizontalComparisons[1][5], verticalComparisons[0][5], verticalComparisons[0][6]}),
			horizontalBar(verticalComparisons[0][6]),
			horizontalBar(verticalComparisons[0][6]),
			horizontalBar(verticalComparisons[0][6]),
			cross((bool[4]){horizontalComparisons[0][6], horizontalComparisons[1][6], verticalComparisons[0][6], verticalComparisons[0][7]}),
			horizontalBar(verticalComparisons[0][7]),
			horizontalBar(verticalComparisons[0][7]),
			horizontalBar(verticalComparisons[0][7]),
			cross((bool[4]){horizontalComparisons[0][7], horizontalComparisons[1][7], verticalComparisons[0][7], verticalComparisons[0][8]}),
			horizontalBar(verticalComparisons[0][8]),
			horizontalBar(verticalComparisons[0][8]),
			horizontalBar(verticalComparisons[0][8]),
			cross((bool[4]){horizontalComparisons[0][8], horizontalComparisons[1][8], verticalComparisons[0][8], verticalComparisons[0][9]}),
			horizontalBar(verticalComparisons[0][9]),
			horizontalBar(verticalComparisons[0][9]),
			horizontalBar(verticalComparisons[0][9]),
			rightTWall(verticalComparisons[0][9])},

		{L'║', L' ', starsInput[1][0], L' ', verticalBar(horizontalComparisons[1][0]), L' ', starsInput[1][1], L' ', L'║'},

		{L'╚', L'═', L'═', L'═', TFloor(horizontalComparisons[1][0]), L'═', L'═', L'═', L'╝'}

	};
	
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

		if ((GetAsyncKeyState('H') & 0x8000) && cursorLocation[0] > 2) {
			cursorLocation[0] -= 4;
			position.X = cursorLocation[0];
    			SetConsoleCursorPosition(hStdout, position);	
			while (GetAsyncKeyState('H') & 0x8000);

		}

		if ((GetAsyncKeyState('J') & 0x8000) && cursorLocation[1] < 19) {
			cursorLocation[1] += 2;
			position.Y = cursorLocation[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while (GetAsyncKeyState('J') & 0x8000);
		}

		if ((GetAsyncKeyState('K') & 0x8000) && cursorLocation[1] > 1) {
			cursorLocation[1] -= 2;
			position.Y = cursorLocation[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while (GetAsyncKeyState('K') & 0x8000);
		}

		if ((GetAsyncKeyState('L') & 0x8000) && cursorLocation[0] < 38) {
			cursorLocation[0] += 4;
			position.X = cursorLocation[0];
    			SetConsoleCursorPosition(hStdout, position);
 			while (GetAsyncKeyState('L') & 0x8000);
		}

	}
	return 0;
}
