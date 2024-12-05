#include <iostream>
#include <windows.h>
#include <conio.h>

#include "Board.h"
#include "Utils.h"
#include "Mario.h"

// Better put this is a proper class
constexpr int ESC = 27;

int main() {
	ShowConsoleCursor(false);
	Board board;
	board.reset();
	board.print();
	Mario mario(&board);
	mario.draw();
	while (true) {
		Sleep(50);
		mario.erase();
		if (_kbhit()) {
			char key = _getch();
			if (key == ESC) break;
			mario.keyPressed(key);
		}
		mario.move();
		mario.draw();
	}
}