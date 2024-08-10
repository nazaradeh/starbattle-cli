#include "generate.hpp"
#include "globals.hpp"

std::array<std::array<int, 10>, 10> regions;

void generatePuzzle() {
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
}