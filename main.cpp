#include "grid.hpp"
#include <conio.h>
#include <thread>

std::array<std::array<int, 10>, 10> regions;
std::array<std::array<CellState, 10>, 10> cellStates = {EMPTY};
std::array<int, 10> starsInRow = {0};
std::array<int, 10> starsInColumn = {0};
std::array<int, 10> starsInRegion = {0};
std::pair<int, int> cursorLocationActual = {2, 3};
std::pair<int, int> cursorLocationGameplay() {return {(cursorLocationActual.first - 1) / 2, (cursorLocationActual.second - 2) / 4};}

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
	
	std::print("\x1b[?1049h{}\033[2;3H", buildGrid());
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	while (true) {

		if (kbhit()) { 

			char c = getch();
	
			// ESCAPE
			if (c == 27) {
				std::print("\x1b[?1049l");
				break;
			}

			// LEFT
			else if ((c == 75 || c == 'a' || c == 'h') && cursorLocationActual.second > 2) {
				cursorLocationActual.second -= 4;
				std::print("\033[4D");
			}

			// DOWN
			else if ((c == 80 || c == 's' || c == 'j') && cursorLocationActual.first < 19) {
				cursorLocationActual.first += 2;
				std::print("\033[2B");
			}

			// UP
			else if ((c == 72 || c == 'w' || c == 'k') && cursorLocationActual.first > 1) {
				cursorLocationActual.first -= 2;
				std::print("\033[2A");
			}

			// RIGHT
			else if ((c == 77 || c == 'd' || c == 'l') && cursorLocationActual.second < 38) {
				cursorLocationActual.second += 4;
				std::print("\033[4C");
			}

			// ENTER, SPACE
			else if (c == 13 || c == ' ') {
				
				// Add ·
				if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == EMPTY) {
					cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = MARKEDOFF;
				}

				// Add ★
				else if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == MARKEDOFF) {
					cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = STAR;
					++starsInRow[cursorLocationGameplay().first];
					++starsInColumn[cursorLocationGameplay().second];
					++starsInRegion[regions[cursorLocationGameplay().first][cursorLocationGameplay().second]];
				}

				// Clear cell
				else if (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] == STAR) {
					cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = EMPTY; 
					--starsInRow[cursorLocationGameplay().first];
					--starsInColumn[cursorLocationGameplay().second];
					--starsInRegion[regions[cursorLocationGameplay().first][cursorLocationGameplay().second]];
				}

				std::print("{}\033[{};{}H", buildGrid(), cursorLocationActual.first, cursorLocationActual.second);

			}

			else if (c == 'r') {
				solvePuzzle();
				std::print("{}\033[{};{}H", buildGrid(), cursorLocationActual.first, cursorLocationActual.second);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	return 0;

}
