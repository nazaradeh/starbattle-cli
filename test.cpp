#include <iostream>
#include <windows.h>

wchar_t horizontalBar(bool b) {
	return b ? L'─' : L'═'; 
}

wchar_t verticalBar(bool b) {
	return b ? L'│' : L'║';
}

wchar_t leftTWall(bool b) {
	return b ? L'╟' : L'╠';
}

wchar_t rightTWall(bool b) {
	return b ? L'╢' : L'╣';
}

wchar_t TCeiling(bool b) {
	return b ? L'╤' : L'╦';
}

wchar_t TFloor(bool b) {
	return b ? L'╧' : L'╩';
}

wchar_t cross(bool b[4]) {
	// {top comparison, bottom comparison, left comparison, right comparison}

	// {0, 0, 0, x}
	if (b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0) {return L'╬';}
	else if (b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 1) {return L'╣';}
	
	// {0, 0, 1, x}
	else if (b[0] == 0 && b[1] == 0 && b[2] == 1 && b[3] == 0) {return L'╠';}
	else if (b[0] == 0 && b[1] == 0 && b[2] == 1 && b[3] == 1) {return L'╫';}	

	// {0, 1, x, x}
	else if (b[0] == 0 && b[1] == 1 && b[2] == 0 && b[3] == 0) {return L'╩';}
	else if (b[0] == 0 && b[1] == 1 && b[2] == 1 && b[3] == 0) {return L'╚';}	
	else if (b[0] == 0 && b[1] == 1 && b[2] == 0 && b[3] == 1) {return L'╝';}	
	//not possible (b[0] == 0 && b[1] == 1 && b[2] == 1 && b[3] == 1)
	
	// {1, 0, x, x}
	else if (b[0] == 1 && b[1] == 0 && b[2] == 0 && b[3] == 0) {return L'╦';}
	else if (b[0] == 1 && b[1] == 0 && b[2] == 1 && b[3] == 0) {return L'╔';}
	else if (b[0] == 1 && b[1] == 0 && b[2] == 0 && b[3] == 1) {return L'╗';}
	// not possible (b[0] == 1 && b[1] == 0 && b[2] == 1 && b[3] == 1)

	// {1, 1, x, x}
	else if (b[0] == 1 && b[1] == 1 && b[2] == 0 && b[3] == 0) {return L'╪';}
	// not possible (b[0] == 1 && b[1] == 1 && b[2] == 1 && b[3] == 0)
	// not possible (b[0] == 1 && b[1] == 1 && b[2] == 0 && b[3] == 1)
	else {return L'┼';}

}


int main() {

	// Switch to the alternate screen buffer.
	std::cout << "\x1b[?1049h";

	SetConsoleOutputCP(CP_UTF8);

	int regions[2][2] = {
		{2, 0},
		{2, 1}
	};


	wchar_t starsInput[2][2] = {
		{L' ', L' '},
		{L' ', L' '}
	};


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

		{leftTWall(verticalComparisons[0][0]), horizontalBar(verticalComparisons[0][0]), horizontalBar(verticalComparisons[0][0]), horizontalBar(verticalComparisons[0][0]), cross((bool[4]){horizontalComparisons[0][0], horizontalComparisons[1][0], verticalComparisons[0][0], verticalComparisons[0][1]}), horizontalBar(verticalComparisons[0][1]), horizontalBar(verticalComparisons[0][1]), horizontalBar(verticalComparisons[0][1]), rightTWall(verticalComparisons[0][1])},

		{L'║', L' ', starsInput[1][0], L' ', verticalBar(horizontalComparisons[1][0]), L' ', starsInput[1][1], L' ', L'║'},

		{L'╚', L'═', L'═', L'═', TFloor(horizontalComparisons[1][0]), L'═', L'═', L'═', L'╝'}

	};
	
	for (wchar_t (&row)[9] : grid) {
		for (wchar_t col : row) {
			std::wcout.imbue(std::locale("en_US.UTF-8"));
			std::wcout << col;
		}
		std::wcout.imbue(std::locale("en_US.UTF-8"));
		std::wcout << std::endl;
	}

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			std::cout << "\x1b[?1049l";
			break;
		}
	}
	return 0;
}
