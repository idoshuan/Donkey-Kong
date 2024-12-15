#include "Screen.h"

void Screen::printScreen(const char** screen) const{
	clearScreen();
	for (int i = 0; i < SCREEN_BOUNDARIES::MAX_Y - 1; ++i) {
		std::cout << screen[i];
		std::cout << '\n'; 
	}
	std::cout << screen[SCREEN_BOUNDARIES::MAX_Y - 1];
}