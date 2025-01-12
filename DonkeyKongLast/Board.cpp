
#include "Board.h"

// ------------------- Public Methods -------------------

/**
 * @brief Resets the game board to its initial state.
 * Copies the contents of `originalBoard` into `currentBoard`.
 */
void Board::reset() {
	memcpy(currentBoard, originalBoard, SCREEN_BOUNDARIES::MAX_Y * (SCREEN_BOUNDARIES::MAX_X + 1));
}

/**
 * @brief Prints the current state of the game board to the console.
 * Clears the screen before printing to ensure the board is displayed cleanly.
 */
void Board::print() const {
    clearScreen();
	std::cout << currentBoard[0];
}

void Board::load(const std::string& filename) {
	std::ifstream screenFile(filename);
	if (screenFile.bad()) {
		//TO-DO return error
	}
	else {
		int currRow = 0;
		int currCol = 0;
		char c;
		bool isBounded = false;

		while (!screenFile.get(c).eof() && currRow < SCREEN_BOUNDARIES::MAX_Y) {
			if (c == '\n') {
				if (currCol < SCREEN_BOUNDARIES::MAX_X) {
					#pragma warning(suppress : 4996) 
					strcpy(originalBoard[currRow] + currCol, std::string(SCREEN_BOUNDARIES::MAX_X - currCol - 1, ' ').c_str());
				}
				++currRow;
				currCol = 0;
				continue;
			}
			if (currCol < SCREEN_BOUNDARIES::MAX_X) {
				if (std::toupper(c) == BOARD_CHARACTERS::QFLOOR && !isBounded) {
					isBounded = true;
				}
				else if (c == ENTITIES_CHARACTERS::MARIO) {
					marioPos = { currCol, currRow };
					c = ' ';
				}
				else if (c == ENTITIES_CHARACTERS::PAULINA) {
					paulinaPos = { currCol, currRow };
				}
				else if (c == ENTITIES_CHARACTERS::DONKEY_KONG) {
					donkeyPos = { currCol, currRow };
				}
				else if (std::tolower(c) == BOARD_CHARACTERS::HAMMER) {
					hammerPos = { currCol, currRow };
				}
				else if (std::toupper(c) == ENTITIES_CHARACTERS::GHOST) {
					ghostsPos.push_back({ currCol, currRow });
					c = ' ';
				}
				originalBoard[currRow][currCol++] = c;
			}
		}
		int lastRow = (currRow < SCREEN_BOUNDARIES::MAX_Y ? currRow : SCREEN_BOUNDARIES::MAX_Y - 1);
		if (isBounded) {
			#pragma warning(suppress : 4996) 
			strcpy(originalBoard[0], std::string(SCREEN_BOUNDARIES::MAX_X, 'Q').c_str());
			#pragma warning(suppress : 4996) 
			strcpy(originalBoard[lastRow], std::string(SCREEN_BOUNDARIES::MAX_X, 'Q').c_str());
			for (int row = 1; row < lastRow; ++row) {
				originalBoard[row][0] = 'Q';
				originalBoard[row][SCREEN_BOUNDARIES::MAX_X - 1] = 'Q';
				originalBoard[row][SCREEN_BOUNDARIES::MAX_X] = '\n';
			}
		}
		originalBoard[0][SCREEN_BOUNDARIES::MAX_X] = '\n';
		originalBoard[lastRow][SCREEN_BOUNDARIES::MAX_X] = '\0';
		for (int row = 1; row < lastRow; ++row) {
			originalBoard[row][SCREEN_BOUNDARIES::MAX_X] = '\n';
		}
	}
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

