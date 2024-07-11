#include <iostream>
#include <windows.h>

wchar_t horizontalBar(bool b) {return b ? L'─' : L'═';}

wchar_t verticalBar(bool b) {return b ? L'│' : L'║';}

wchar_t leftTWall(bool b) {return b ? L'╟' : L'╠';}

wchar_t rightTWall(bool b) {return b ? L'╢' : L'╣';}

wchar_t TCeiling(bool b) {return b ? L'╤' : L'╦';}

wchar_t TFloor(bool b) {return b ? L'╧' : L'╩';}

wchar_t cross(bool b[4]) {
	char crossComparisons = b[0] << 3 | b[1] << 2 | b[2] << 1 | b[0];
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


	int regions[2][2] = {
		{0, 0},
		{1, 0}
	};


	wchar_t starsInput[2][2] = {
		{L' ', L' '},
		{L' ', L' '}
	};


	int cursorLocation[2] = {2, 1};


	bool verticalComparisons[1][2] = {
		{regions[0][0] == regions[1][0], regions[0][1] == regions[1][1]}
	};


	bool horizontalComparisons[2][1] = {
		{regions[0][0] == regions[0][1]},
		{regions[1][0] == regions[1][1]}
	};
	
	
	wchar_t grid[5][9] = {

		{L'╔', L'═', L'═', L'═', TCeiling(horizontalComparisons[0][0]), L'═', L'═', L'═', L'╗'},

		{L'║', L' ', starsInput[0][0], L' ', verticalBar(horizontalComparisons[0][0]), L' ', starsInput[0][1], L' ', L'║'},

		{leftTWall(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			horizontalBar(verticalComparisons[0][0]),
			cross((bool[4]){horizontalComparisons[0][0], horizontalComparisons[1][0], verticalComparisons[0][0], verticalComparisons[0][1]}),
			horizontalBar(verticalComparisons[0][1]),
			horizontalBar(verticalComparisons[0][1]),
			horizontalBar(verticalComparisons[0][1]),
			rightTWall(verticalComparisons[0][1])},

		{L'║', L' ', starsInput[1][0], L' ', verticalBar(horizontalComparisons[1][0]), L' ', starsInput[1][1], L' ', L'║'},

		{L'╚', L'═', L'═', L'═', TFloor(horizontalComparisons[1][0]), L'═', L'═', L'═', L'╝'}

	};
	
	for (wchar_t (&row)[9] : grid) {
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

		if ((GetAsyncKeyState('J') & 0x8000) && cursorLocation[1] < 3) {
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

		if ((GetAsyncKeyState('L') & 0x8000) && cursorLocation[0] < 6) {
			cursorLocation[0] += 4;
			position.X = cursorLocation[0];
    			SetConsoleCursorPosition(hStdout, position);
 			while (GetAsyncKeyState('L') & 0x8000);
		}

	}
	return 0;
}
