#include "grid.hpp"
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <array>
#include <utility>
//#include <chrono>

constexpr wchar_t STAR = L'★';

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


	std::array<std::array<wchar_t, 10>, 10> starsInput;	
	for (auto& row : starsInput) {std::fill(std::begin(row), std::end(row), L' ');}

	// To see if there are too many stars
	std::array<int, 10> starsInRow = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	std::array<int, 10> starsInColumn = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	std::array<int, 10> starsInRegion = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::wstring grid = buildGrid(regions); // Draw the grid visually on the console via grid.cpp
	
	// Required for displaying box drawing characters
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));
	//std::wcout << L"\x1b[?1049h\x1b[H" + grid; //Switch to alternate screen buffer, move to top of the screen, and print grid.
	std::wcout << L"\x1b[?1049h\x1b[H" + grid; //Switch to alternate screen buffer, move to top of the screen, and print grid.

    	// Cursor position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorLocationActual = {2, 1};
    	SetConsoleCursorPosition(hStdout, cursorLocationActual);
	std::pair<int, int> cursorLocationGameplay = {0, 0}; // Where we are in relation to the cells

	// Controls
	//enum keyState {RELEASED, PRESSED, HELD};
	//std::chrono::steady_clock::time_point pressedTime;
	//std::chrono::steady_clock::time_point heldTime;
	Sleep(100);
	while (true) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::cout << "\x1b[?1049l";
			break;
		}

		if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('H')) && cursorLocationActual.X > 2) {
			cursorLocationActual.X -= 4;
			cursorLocationGameplay.first -= 1;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);	
			while ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState('H') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_DOWN) ||GetAsyncKeyState('S') ||  GetAsyncKeyState('J')) && cursorLocationActual.Y < 19) {
			cursorLocationActual.Y += 2;
			cursorLocationGameplay.second += 1;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('J') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('K')) && cursorLocationActual.Y > 1) {
			cursorLocationActual.Y -= 2;
			cursorLocationGameplay.second -= 1;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('K') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('L')) && cursorLocationActual.X < 38) {
			cursorLocationActual.X += 4;
			cursorLocationGameplay.first += 1;
    			SetConsoleCursorPosition(hStdout, cursorLocationActual);
 			while ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState('L') & 0x8000));
		}

		if ((GetAsyncKeyState(VK_RETURN) & 0x8000) || GetAsyncKeyState(VK_SPACE)) {

			// Add ·
			if (starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] == L' ') {
				starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] = L'·';
				std::wcout << "\x1b[90m" << L'·' << '\b';
			}
			
			// Add ★
			else if (starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] == L'·') {

				starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] = STAR;
				
				//++starsInRow[cursorLocationGameplay.second];
				//++starsInColumn[cursorLocationGameplay.first];
				//++starsInRegion[regions[cursorLocationGameplay.second][cursorLocationGameplay.first]];
				
				// Always check adjacent cells for conflicting stars
				//COORD RedStarPrint = cursorLocationActual;
				for (int xOff = -1; xOff < 2; ++xOff) {
				
					if (starsInput[2][1] == STAR) {
						SetConsoleCursorPosition(hStdout, {2, 1});
						std::cout << 'a';
					}
					/*for (int yOff = -1; yOff < 2; ++yOff) {
						if (xOff != 1 && yOff != 0 && starsInput[cursorLocationGameplay.first + xOff][cursorLocationGameplay.second + yOff] == STAR) {
							RedStarPrint.X += 4 * xOff;
							RedStarPrint.Y += 2 * yOff;
							SetConsoleCursorPosition(hStdout, RedStarPrint);
							std::wcout << "\x1b[31m" << STAR;
						}
					}*/
				}
				SetConsoleCursorPosition(hStdout, cursorLocationActual);

				// Three ★ in row or column
				if (starsInRow[cursorLocationGameplay.second] == 3 || starsInColumn[cursorLocationGameplay.first] == 3) {

					std::wcout << "\x1b[31m" << STAR << '\b';

					// Three ★ in row
					if (starsInRow[cursorLocationGameplay.second] == 3) {
						int starsReddened = 0;
						COORD positionForPrintingRedStar = {2, cursorLocationActual.Y};
						for (int col = 0; col < starsInput.size(); ++col) {
							if (col != cursorLocationGameplay.first && starsInput[col][cursorLocationGameplay.second] == STAR) {
								SetConsoleCursorPosition(hStdout, positionForPrintingRedStar);
								std::wcout << STAR << '\b';
								++starsReddened;
								if (starsReddened >= 2) {break;}
							}
							positionForPrintingRedStar.X += 4;
						}
						SetConsoleCursorPosition(hStdout, cursorLocationActual);
					}
				
					// Three ★ in column
					if (starsInColumn[cursorLocationGameplay.first] == 3) {
						int starsReddened = 0;
						COORD positionForPrintingRedStar = {cursorLocationActual.X, 1};
						for (int row = 0; row < starsInput.size(); ++row) {
							if (row != cursorLocationGameplay.second && starsInput[cursorLocationGameplay.first][row] == STAR) {
								SetConsoleCursorPosition(hStdout, {static_cast<SHORT>(cursorLocationActual.X), static_cast<SHORT>(row * 2 + 1)});
								std::wcout << STAR << '\b';
								++starsReddened;
								if (starsReddened >= 2) {break;}
							}
							positionForPrintingRedStar.Y += 2;
						}
						SetConsoleCursorPosition(hStdout, cursorLocationActual);
					}

				}

				// Four or more ★ in row or column
				else if (starsInRow[cursorLocationGameplay.second] > 3 || starsInColumn[cursorLocationGameplay.first] > 3) {std::wcout << "\x1b[31m" << STAR << '\b';}

				else {
					for (int xOff = -1; xOff < 2; ++xOff) {
						for (int yOff = -1; yOff < 2; ++yOff) {
							if (xOff != 1 && yOff != 0 && starsInput[cursorLocationGameplay.first + xOff][cursorLocationGameplay.second + yOff] == STAR) {
								SetConsoleCursorPosition(hStdout, {static_cast<SHORT>(cursorLocationActual.X + 4 * xOff), static_cast<SHORT>(cursorLocationActual.Y + 2 * yOff)});
								std::wcout << "\x1b[31m" << STAR << '\b';
							}
						}
					}
					std::wcout << "\x1b[33m" << STAR << '\b';
				}
			}
			else if (starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] == STAR) {
				starsInput[cursorLocationGameplay.first][cursorLocationGameplay.second] = L' ';
				--starsInRow[cursorLocationGameplay.second];
				--starsInColumn[cursorLocationGameplay.first];
				--starsInRegion[regions[cursorLocationGameplay.second][cursorLocationGameplay.first]];
				std::cout << ' ' << '\b';

				// Back down to two in row or column
				if (starsInRow[cursorLocationGameplay.second] == 2 || starsInColumn[cursorLocationGameplay.first] == 2) {

					// Down to two ★ in row
					if (starsInRow[cursorLocationGameplay.second] == 2) {
						int starsRestored = 0;
						COORD positionForPrintingRedStar = {2, cursorLocationActual.Y};
						for (int col = 0; col < starsInput.size(); ++col) {
							if (starsInput[col][cursorLocationGameplay.second] == STAR) {
								if (starsInColumn[col] < 3) {
									SetConsoleCursorPosition(hStdout, positionForPrintingRedStar);
									std::wcout << "\x1b[33m" << STAR << '\b';
								}
								++starsRestored;
								if (starsRestored >= 2) {break;}
							}
							positionForPrintingRedStar.X += 4;
						}
						SetConsoleCursorPosition(hStdout, cursorLocationActual);
					}
				
					// Down to two ★ in column
					if (starsInColumn[cursorLocationGameplay.first] == 2) {
						int starsReddened = 0;
						COORD positionForPrintingRedStar = {cursorLocationActual.X, 1};
						for (int row = 0; row < starsInput.size(); ++row) {
							if (starsInput[cursorLocationGameplay.first][row] == STAR) {
								if (starsInRow[row] < 3) {
									SetConsoleCursorPosition(hStdout, positionForPrintingRedStar);
									std::wcout << "\x1b[33m" << STAR << '\b';
								}
								++starsReddened;
								if (starsReddened >= 2) {break;}
							}
							positionForPrintingRedStar.Y += 2;
						}
						SetConsoleCursorPosition(hStdout, cursorLocationActual);
					}

				}

			}
 			while ((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000));
		}

		Sleep(10);

	}
	return 0;
}
