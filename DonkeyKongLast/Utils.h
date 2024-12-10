#pragma once
#include <Windows.h>

void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
inline void clearScreen() {
	system("cls");
}