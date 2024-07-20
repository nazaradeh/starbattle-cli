#include "grid.hpp"
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <array>
#include <utility>

std::array<std::array<CellState, 10>, 10> cellStates;

// To see if there are too many stars
std::array<int, 10> starsInRow;
std::array<int, 10> starsInColumn;
std::array<int, 10> starsInRegion;

COORD cursorLocationActual = {2, 1};
std::pair<int, int> cursorLocationGameplay() {return std::make_pair((cursorLocationActual.X - 2) / 4, (cursorLocationActual.Y - 1) / 2);}
int stringLocation() {return 49 + 246 * cursorLocationGameplay().second + 20 * cursorLocationGameplay().first;}

int main() {
	std::array<std::array<int, 10>, 10> regions = {{
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
	}};
	
	for (auto& row : cellStates) {std::fill(std::begin(row), std::end(row), EMPTY);}

	std::wstring grid = buildGrid(regions);

	// Required for displaying box drawing characters
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));
	std::wcout << L"\x1b[?1049h" + grid; //Switch to alternate screen buffer and print grid.
	
	starsInRow = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	starsInColumn = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	starsInRegion = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	

    	// Cursor position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleCursorPosition(hStdout, cursorLocationActual);
	char keyPress;
	// Controls
	Sleep(200);
	while (true) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::cout << "\x1b[?1049l";
			break;
		}

		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('H')) && cursorLocationActual.X > 2) {
			cursorLocationActual.X -= 4;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);	
			while ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('H') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_DOWN) ||GetAsyncKeyState('S') ||  GetAsyncKeyState('J')) && cursorLocationActual.Y < 19) {
			cursorLocationActual.Y += 2;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('J') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('K')) && cursorLocationActual.Y > 1) {
			cursorLocationActual.Y -= 2;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('K') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('L')) && cursorLocationActual.X < 38) {
			cursorLocationActual.X += 4;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('L') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || GetAsyncKeyState(VK_SPACE)) {
			
			// Add ·
			if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == EMPTY) {
				cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = MARKEDOFF;
			}

			// Add ★
			else if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == MARKEDOFF) {
				cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = STAR;
				++starsInRow[cursorLocationGameplay().second];
				++starsInColumn[cursorLocationGameplay().first];
				++starsInRegion[regions[cursorLocationGameplay().second][cursorLocationGameplay().first]];
			}

			// Clear cell
			else if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == STAR) {
				cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = EMPTY; 
				--starsInRow[cursorLocationGameplay().second];
				--starsInColumn[cursorLocationGameplay().first];
				--starsInRegion[regions[cursorLocationGameplay().second][cursorLocationGameplay().first]];
			}

			grid.clear();
			grid = buildGrid(regions);
			std::wcout << grid << std::endl;
			SetConsoleCursorPosition(hStdout, cursorLocationActual);

 			while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000));
		}

		Sleep(10);

	}
	return 0;
}
