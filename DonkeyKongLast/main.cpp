#include <iostream>
#include <windows.h>
#include <conio.h>

#include "Board.h"
#include "Utils.h"
#include "Mario.h"
#include "Barrel.h"
// Better put this is a proper class
constexpr int ESC = 27;

int main() {
	ShowConsoleCursor(false);
	Board board;
	Barrel barrel(30, 2, &board);
	board.reset();
	board.print();
	Mario mario(&board);
	mario.draw();
	barrel.draw();
	while (true) {
		Sleep(100);
		mario.erase();
		barrel.erase();
		if (_kbhit()) {
			char key = _getch();
			if (key == ESC) break;
			mario.keyPressed(key);
		}
		mario.move();
		barrel.move();
		mario.draw();
		barrel.draw();
	}
}
