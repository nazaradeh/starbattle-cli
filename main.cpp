#include "grid.hpp"
//#include "curses.h"
#include <windows.h>
//#include <array>
//#include <chrono>
#include <thread>
//#include <print>

std::array<std::array<int, 10>, 10> regions;
std::array<std::array<CellState, 10>, 10> cellStates = {EMPTY};
std::array<int, 10> starsInRow = {0};
std::array<int, 10> starsInColumn = {0};
std::array<int, 10> starsInRegion = {0};
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
	
	std::print("\x1b[?1049h{}", buildGrid());
	
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleCursorPosition(hStdout, cursorLocationActual);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	while (true) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::print("\x1b[?1049l");
			break;
		}

		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('H')) && cursorLocationActual.X > 2) {
			cursorLocationActual.X -= 4;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);	
			while ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('H') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('W') || GetAsyncKeyState('J')) && cursorLocationActual.Y < 19) {
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
			if (cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] == EMPTY) {
				cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] = MARKEDOFF;
			}

			// Add ★
			else if (cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] == MARKEDOFF) {
				cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] = STAR;
				++starsInRow[cursorLocationGameplay().second];
				++starsInColumn[cursorLocationGameplay().first];
				++starsInRegion[regions[cursorLocationGameplay().second][cursorLocationGameplay().first]];
			}

			// Clear cell
			else if (cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] == STAR) {
				cellStates[cursorLocationGameplay().second][cursorLocationGameplay().first] = EMPTY; 
				--starsInRow[cursorLocationGameplay().second];
				--starsInColumn[cursorLocationGameplay().first];
				--starsInRegion[regions[cursorLocationGameplay().second][cursorLocationGameplay().first]];
			}

			std::print("{}", buildGrid());
			SetConsoleCursorPosition(hStdout, cursorLocationActual);

 			while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000));
		}

		if (GetAsyncKeyState('R')) {
			solvePuzzle();
			std::print("{}", buildGrid());
			while (GetAsyncKeyState('R'));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	return 0;
}
