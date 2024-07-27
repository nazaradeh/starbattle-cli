#include "grid.hpp"
//#include "curses.h"
#include <iostream>
#include <windows.h>
#include <array>
#include <chrono>
#include <thread>

std::array<std::array<int, 10>, 10> regions;
std::array<std::array<CellState, 10>, 10> cellStates;
std::array<int, 10> starsInRow;
std::array<int, 10> starsInColumn;
std::array<int, 10> starsInRegion;
COORD cursorLocationActual = {2, 1};
std::pair<int, int> cursorLocationGameplay() {return std::make_pair((cursorLocationActual.X - 2) / 4, (cursorLocationActual.Y - 1) / 2);}

int main() {
	regions = {{
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
	
	for (auto& row : cellStates) {row.fill(EMPTY);}

	std::wstring grid = buildGrid();

	/*
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	timeout(0);
	
	printw("%ls", grid.c_str());
	refresh();
	*/

	// Required for displaying box drawing characters
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));
	std::wcout << L"\x1b[?1049h" + grid; //Switch to alternate screen buffer and print grid.
	
	starsInRow.fill(0);
	starsInColumn.fill(0);
	starsInRegion.fill(0);

    	// Cursor position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleCursorPosition(hStdout, cursorLocationActual);
	// Controls
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
			grid = buildGrid();
			std::wcout << grid << std::endl;
			SetConsoleCursorPosition(hStdout, cursorLocationActual);

 			while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000));
		}

		if (GetAsyncKeyState('0')) {
			while (GetAsyncKeyState('0'));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	return 0;
}
