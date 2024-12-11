#include <cstring>
#include <iostream>

#include "Board.h"

void Board::reset() {
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
	}
}

void Board::print() const {

	for (int y = 0; y < MAX_Y - 1; ++y) {
		for (int x = 0; x < MAX_X - 1; ++x) {
			Point currentPoint(x, y);
			if (currentPoint == paulina) {
				std::cout << char(ENTITIES_CHARACTERS::PRINCESS);
			}
			else if (currentPoint == donkeyKong) {
				std::cout << char(ENTITIES_CHARACTERS::DONKEY_KONG);
			}
			else {
				std::cout << currentBoard[y][x];
			}
		}
		std::cout << std::endl;
	}


	std::cout << currentBoard[MAX_Y - 1];


}

