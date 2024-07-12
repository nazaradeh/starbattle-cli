#include "grid.h"
#include <iostream>
#include <windows.h>
#include <algorithm>

int main() {

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

	drawGrid(regions); // Draw the grid visually on the console via grid.cpp

	int cursorLocationActual[2] = {2, 1};
	int cursorLocationGameplay[2] = {0, 0};


    	// Cursor position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	// Create a COORD struct to hold the cursor position
    	COORD position;
	position.X = cursorLocationActual[0];  // X coordinate (0-based index)
	position.Y = cursorLocationActual[1];  // Y coordinate (0-based index)
    	// Set the console cursor position
    	SetConsoleCursorPosition(hStdout, position);

	// Controls
	while (true) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::cout << "\x1b[?1049l";
			break;
		}

		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('H')) && cursorLocationActual[0] > 2) {
			cursorLocationActual[0] -= 4;
			cursorLocationGameplay[0] -= 1;
			position.X = cursorLocationActual[0];
    			SetConsoleCursorPosition(hStdout, position);	
			while ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('H') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_DOWN) ||GetAsyncKeyState('S') ||  GetAsyncKeyState('J')) && cursorLocationActual[1] < 19) {
			cursorLocationActual[1] += 2;
			cursorLocationGameplay[1] += 1;
			position.Y = cursorLocationActual[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('J') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('K')) && cursorLocationActual[1] > 1) {
			cursorLocationActual[1] -= 2;
			cursorLocationGameplay[1] -= 1;
			position.Y = cursorLocationActual[1];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('K') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('L')) && cursorLocationActual[0] < 38) {
			cursorLocationActual[0] += 4;
			cursorLocationGameplay[0] += 1;
			position.X = cursorLocationActual[0];
    			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('L') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || GetAsyncKeyState(VK_SPACE)) {
			if (starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] == L' '){starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] = L'★';}
			else if (starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] == L'★'){starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] = L'×';}
			else if (starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] == L'×'){starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]] = L' ';}
			std::wcout << starsInput[cursorLocationGameplay[0]][cursorLocationGameplay[1]];
			SetConsoleCursorPosition(hStdout, position);
 			while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState('L') & 0x8000));
		}

		Sleep(10);

	}
	return 0;
}
