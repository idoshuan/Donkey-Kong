
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

bool Board::load(const std::string& filename, std::string* errors) {
	std::string errorMessages;
	std::ifstream screenFile(filename);
	bool isValid = true;

	if (screenFile.bad()) {
		errorMessages += "Error opening file: " + filename + "\n";
		return false;
	}

	bool hasMario = false, hasPaulina = false, hasDonkey = false, hasHammer = false;
	bool isBounded = false;
	int currRow = 0, currCol = 0;
	char c;

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

		else if (currCol < SCREEN_BOUNDARIES::MAX_X) {
			if (c == ENTITIES_CHARACTERS::MARIO) {
				if (hasMario) {
					errorMessages += "Error: Multiple Mario positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
					isValid = false;
				}
				else {
					marioPos = { currCol, currRow };
					hasMario = true;
				}
				c = ' ';
			}
			else if (std::tolower(c) == ENTITIES_CHARACTERS::GHOST) {
				ghostsPos.push_back({ currCol, currRow });
				c = ' ';
			}
			else if (c == ENTITIES_CHARACTERS::PAULINA) {
				if (hasPaulina) {
					errorMessages += "Error: Multiple Paulina positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
					isValid = false;
				}
				else {
					paulinaPos = { currCol, currRow };
					hasPaulina = true;
				}
			}
			else if (c == ENTITIES_CHARACTERS::DONKEY_KONG) {
				if (hasDonkey) {
					errorMessages += "Error: Multiple Donkey positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
					isValid = false;
				}
				else {
					donkeyPos = { currCol, currRow };
					hasDonkey = true;
				}
			}
			else if (std::toupper(c) == BOARD_CHARACTERS::QFLOOR) {
				isBounded = true;
			}
			else if (std::tolower(c) == BOARD_CHARACTERS::HAMMER) {
				if (!hasHammer) {
					hammerPos = { currCol, currRow };
					hasHammer = true;
				}
				else {
					isValid = false;
				}
			}
			else if (
				c == BOARD_CHARACTERS::FLOOR ||
				c == BOARD_CHARACTERS::RIGHT_FLOOR ||
				c == BOARD_CHARACTERS::LEFT_FLOOR ||
				c == BOARD_CHARACTERS::AIR ||
				std::toupper(c) == BOARD_CHARACTERS::LADDER ||
				std::toupper(c) == BOARD_CHARACTERS::LEGEND) // move and make legend pos
			{
				;
			}
			else {
				errorMessages += "Error: Unknown character '" + std::string(1, c) + "' at position (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
				isValid = false;
			}
			originalBoard[currRow][currCol++] = c;
		}
	}


	if (!hasMario) errorMessages += "Error: Mario doesn't exist\n", isValid = false;
	if (!hasDonkey) errorMessages += "Error: Donkey doesn't exist\n", isValid = false;
	if (!hasPaulina) errorMessages += "Error: Paulina doesn't exist\n", isValid = false;

	int lastRow = SCREEN_BOUNDARIES::MAX_Y - 1;
	if (currRow < lastRow || !std::all_of(originalBoard[lastRow], originalBoard[lastRow] + SCREEN_BOUNDARIES::MAX_X,
		[](char c) { return c == BOARD_CHARACTERS::FLOOR || c == BOARD_CHARACTERS::LEFT_FLOOR || c == BOARD_CHARACTERS::RIGHT_FLOOR || c == BOARD_CHARACTERS::QFLOOR; })) {
		errorMessages += "Error: Bottom row is not a valid floor\n";
		isValid = false;
	}

	if (isBounded) {
		memset(originalBoard[0], 'Q', SCREEN_BOUNDARIES::MAX_X);
		memset(originalBoard[lastRow], 'Q', SCREEN_BOUNDARIES::MAX_X);
		for (int row = 1; row < lastRow; ++row) {
			originalBoard[row][0] = originalBoard[row][SCREEN_BOUNDARIES::MAX_X - 1] = 'Q';
		}
	}

	originalBoard[0][SCREEN_BOUNDARIES::MAX_X] = '\n';
	originalBoard[lastRow][SCREEN_BOUNDARIES::MAX_X] = '\0';
	for (int row = 1; row < lastRow; ++row) {
		originalBoard[row][SCREEN_BOUNDARIES::MAX_X] = '\n';
	}

	*errors = errorMessages;
	return isValid;
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

