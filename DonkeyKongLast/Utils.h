#pragma once
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <random>

/**
 * @brief Utility functions for console manipulation.
 * These functions handle cursor positioning, clearing the screen,
 * and toggling the visibility of the console cursor.
 * Note:A Class showed to us in class
 */

 // ------------------- Function Declarations -------------------

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);

double getRandomDouble(double min = 0.0, double max = 1.0);

inline void clearScreen() {
    system("cls");
}
inline void eatBuffer() {
	while (_kbhit()) {
		_getch();
	}
}

