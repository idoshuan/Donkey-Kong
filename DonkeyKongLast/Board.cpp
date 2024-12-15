#include <cstring>
#include <iostream>
#include "Board.h"

// ------------------- Public Methods -------------------

/**
 * @brief Resets the game board to its initial state.
 * Copies the contents of `originalBoard` into `currentBoard`.
 */
void Board::reset() {
    for (int i = 0; i < SCREEN_BOUNDARIES::MAX_Y; i++) {
        memcpy(currentBoard[i], originalBoard[i], SCREEN_BOUNDARIES::MAX_X + 1);
    }
}

/**
 * @brief Prints the current state of the game board to the console.
 * Clears the screen before printing to ensure the board is displayed cleanly.
 */
void Board::print() const {
    clearScreen();
    for (int y = 0; y < SCREEN_BOUNDARIES::MAX_Y - 1; ++y) {
        std::cout << currentBoard[y] << std::endl;
    }
    std::cout << currentBoard[SCREEN_BOUNDARIES::MAX_Y - 1];
}

/**
* @brief Checks if a given position is valid for movement.
* @param p The point to check.
* @return True if the position is valid, false otherwise.
*/
bool Board::isValidPosition(const Point p) const {
	char ch = getChar(p);
	int x = p.getX();
	int y = p.getY();
	return(x >= SCREEN_BOUNDARIES::MIN_X && x < SCREEN_BOUNDARIES::MAX_X && y >= SCREEN_BOUNDARIES::MIN_Y && y < SCREEN_BOUNDARIES::MAX_Y && ch != BOARD_CHARACTERS::FLOOR && ch != BOARD_CHARACTERS::LEFT_FLOOR && ch != BOARD_CHARACTERS::RIGHT_FLOOR);
}

