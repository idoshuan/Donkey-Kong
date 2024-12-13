#include <cstring>
#include <iostream>

#include "Board.h"

void Board::reset() {
	for (int i = 0; i < BOARD_BOUNDARIES::MAX_Y; i++) {
		memcpy(currentBoard[i], originalBoard[i], BOARD_BOUNDARIES::MAX_X + 1);
	}
}

void Board::print() const {

	for (int y = 0; y < BOARD_BOUNDARIES::MAX_Y - 1; ++y) {
		for (int x = 0; x < BOARD_BOUNDARIES::MAX_X - 1; ++x) {
			Point currentPoint(x, y);
			if (currentPoint == CHARACTER_POSITIONS::paulina) {
				std::cout << char(ENTITIES_CHARACTERS::PAULINA);
			}
			else if (currentPoint == CHARACTER_POSITIONS::donkeyKong) {
				std::cout << char(ENTITIES_CHARACTERS::DONKEY_KONG);
			}
			else {
				std::cout << currentBoard[y][x];
			}
		}
		std::cout << std::endl;
	}
	std::cout << currentBoard[BOARD_BOUNDARIES::MAX_Y - 1];
}

