#include "starbattle.hpp"
#include "generate.hpp"
#include "globals.hpp"
#include "grid.hpp"
#include "solver.hpp"
#include <print>
#include <thread>
#ifdef _WIN64
#include <conio.h>
#else
#include <termios.h>
#endif

std::array<std::array<CellState, 10>, 10> cellStates = {EMPTY};
std::array<int, 10> starsInRow = {0};
std::array<int, 10> starsInColumn = {0};
std::array<int, 10> starsInRegion = {0};
std::pair<int, int> cursorLocationActual = {2, 3};
std::pair<int, int> cursorLocationGameplay() {return {(cursorLocationActual.first - 1) / 2, (cursorLocationActual.second - 2) / 4};}

void toggleCellState() {
	switch (cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second]) {
		case EMPTY:
			cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = MARKEDOFF;
			break;
		case MARKEDOFF:
			cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = STAR;
			++starsInRow[cursorLocationGameplay().first];
			++starsInColumn[cursorLocationGameplay().second];
			++starsInRegion[regions[cursorLocationGameplay().first][cursorLocationGameplay().second]];
			break;
		case STAR:
			cellStates[cursorLocationGameplay().first][cursorLocationGameplay().second] = EMPTY; 
			--starsInRow[cursorLocationGameplay().first];
			--starsInColumn[cursorLocationGameplay().second];
			--starsInRegion[regions[cursorLocationGameplay().first][cursorLocationGameplay().second]];
			break;
	}
}

#ifdef _WIN64
void keyInput(bool& exitGame) {
	char c = getch();
	if (c == 27 /*ESCAPE*/ ) {
		std::print("\x1b[?1049l");
		exitGame = true;
	} else if ((c == 75 /*LEFT*/ || c == 'a' || c == 'h') && cursorLocationActual.second > 3) {
		cursorLocationActual.second -= 4;
		std::print("\033[4D");
	} else if ((c == 80 /*DOWN*/ || c == 's' || c == 'j') && cursorLocationActual.first < 19) {
		cursorLocationActual.first += 2;
		std::print("\033[2B");
	} else if ((c == 72 /*UP*/ || c == 'w' || c == 'k') && cursorLocationActual.first > 2) {
		cursorLocationActual.first -= 2;
		std::print("\033[2A");
	} else if ((c == 77 /*RIGHT*/ || c == 'd' || c == 'l') && cursorLocationActual.second < 38) {
		cursorLocationActual.second += 4;
		std::print("\033[4C");
	} else if (c == 13 /*ENTER*/ || c == ' ') {
		toggleCellState();
		std::print("{}\033[{};{}H", buildGrid(), cursorLocationActual.first, cursorLocationActual.second);
	} else if (c == 'r') {
		solvePuzzle();
		std::print("{}\033[{};{}H", buildGrid(), cursorLocationActual.first, cursorLocationActual.second);
	}
}
#endif

void StarBattle::play() {
	generatePuzzle();
	std::print("\x1b[?1049h{}\033[2;3H", buildGrid()); // Go to alternate screen buffer. Move cursor to top.
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	bool exitGame = false;
	while (!exitGame) {
		#ifdef _WIN64
		if (kbhit()) keyInput(exitGame);
		#endif
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}	
}
