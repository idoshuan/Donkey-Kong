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

/**
* @brief Checks if a given position is valid for movement.
* @param p The point to check.
* @return True if the position is valid, false otherwise.
*/
bool Board::isValidPosition(const Point p) const {
	char ch = getChar(p);
	int x = p.getX();
	int y = p.getY();
	return(x >= minX && x < maxX && y >= minY && y < maxY && ch != BOARD_CHARACTERS::FLOOR && ch != BOARD_CHARACTERS::LEFT_FLOOR && ch != BOARD_CHARACTERS::RIGHT_FLOOR && ch != BOARD_CHARACTERS::QFLOOR);
}




// ------------------- Private Methods -------------------

/**
 * @brief Loads a board configuration from a file and validates its structure.
 * @param filename The name of the file to load.
 * @param errors Pointer to a string to store error messages, if any.
 * @return True if the board is valid, false otherwise.
 * Note: This function is based on the load function showed to us in class
 */
bool Board::load(const std::string& filename, std::string* errors) {
	std::string errorMessages;
	std::ifstream screenFile(filename);
	bool isValid = true;

	if (screenFile.bad()) {
		errorMessages += "Error opening file: " + filename + "\n";
		return false;
	}

	bool hasMario = false, hasPaulina = false, hasDonkey = false, hasHammer = false, hasLegend = false;
	bool isBounded = false;
	int currRow = 0, currCol = 0;
	char c;

	while (currRow < SCREEN_BOUNDARIES::MAX_Y&& !screenFile.get(c).eof()) {
		
		if (c == '\n') {
			if (currCol < SCREEN_BOUNDARIES::MAX_X) {
				fillMissingColumns(currCol, currRow);
			}
			originalBoard[currRow][SCREEN_BOUNDARIES::MAX_X] = '\n';
			++currRow;
			currCol = 0;
			continue;
		}
		

		else if (currCol < SCREEN_BOUNDARIES::MAX_X) {
			if (c == ENTITIES_CHARACTERS::MARIO) {
				handleMario(c, currCol, currRow, hasMario, marioPos, isValid, errorMessages);
			}
			else if (std::tolower(c) == ENTITIES_CHARACTERS::GHOST) {
				handleGhost(c, currCol, currRow, ghostsPos);
			}
			else if (c == ENTITIES_CHARACTERS::PAULINA) {
				handlePaulina(c, currCol, currRow, hasPaulina, paulinaPos, isValid, errorMessages);
			}
			else if (c == ENTITIES_CHARACTERS::DONKEY_KONG) {
				handleDonkey(c, currCol, currRow, hasDonkey, donkeyPos, isValid, errorMessages);
			}
			else if (std::toupper(c) == BOARD_CHARACTERS::QFLOOR) {
				handleBorderCharacter(c, currCol, currRow, isBounded, isValid, errorMessages);
			}
			else if (std::tolower(c) == BOARD_CHARACTERS::HAMMER) {
				handleHammer(c, currCol, currRow, hasHammer, hammerPos, isValid);
			}
			else if (std::toupper(c) == BOARD_CHARACTERS::LEGEND) {
				handleLegend(c, currCol, currRow, hasLegend, legendPos, isValid, errorMessages);
			}
			else if (
				c != BOARD_CHARACTERS::FLOOR &&
				c != BOARD_CHARACTERS::RIGHT_FLOOR &&
				c != BOARD_CHARACTERS::LEFT_FLOOR &&
				c != BOARD_CHARACTERS::AIR &&
				std::toupper(c) != BOARD_CHARACTERS::LADDER) {
				handleUnknownCharacter(c, currCol, currRow, isValid, errorMessages);
			}

			originalBoard[currRow][currCol++] = c;
		}
	}

	int lastRow =  c == '\n' ? currRow - 1 : currRow;

	if (lastRow >= 0 && lastRow < SCREEN_BOUNDARIES::MAX_Y) {
		originalBoard[lastRow][SCREEN_BOUNDARIES::MAX_X] = '\0';
	}

	
	if (lastRow < SCREEN_BOUNDARIES::MAX_Y - 1) {
		errorMessages += "Error: Board should include 25 lines";
		isValid = false;
	}
	else {
		if (!std::all_of(originalBoard[lastRow], originalBoard[lastRow] + SCREEN_BOUNDARIES::MAX_X,
			[](char c) { return c == BOARD_CHARACTERS::FLOOR || c == BOARD_CHARACTERS::LEFT_FLOOR || c == BOARD_CHARACTERS::RIGHT_FLOOR || std::toupper(c) == BOARD_CHARACTERS::QFLOOR; })) {
			errorMessages += "Error: Bottom row is not a valid floor\n";
			isValid = false;
		}

		if (!hasMario) errorMessages += "Error: Mario doesn't exist\n", isValid = false;
		else if (!isFloorBelow(marioPos)) errorMessages += "Error: Mario isn't standing on a floor, please insert a valid floor character at position: (" + std::to_string(marioPos.getX()) + ", " + std::to_string(marioPos.getY() + 1) + ")\n", isValid = false;
		if (!hasDonkey) errorMessages += "Error: Donkey doesn't exist\n", isValid = false;
		else if (!isFloorBelow(donkeyPos)) errorMessages += "Error: Donkey isn't standing on a floor, please insert a valid floor character at position: (" + std::to_string(donkeyPos.getX()) + ", " + std::to_string(donkeyPos.getY() + 1) + ")\n", isValid = false;
		if (!hasPaulina) errorMessages += "Error: Paulina doesn't exist\n", isValid = false;
		else if (!isFloorBelow(paulinaPos)) errorMessages += "Error: Paulina isn't standing on a floor, please insert a valid floor character at position: (" + std::to_string(paulinaPos.getX()) + ", " + std::to_string(paulinaPos.getY() + 1) + ")\n", isValid = false;
		if (!hasLegend) errorMessages += "Error: Legend doesn't exist\n", isValid = false;
		if (ghostsPos.size() != 0) {
			for (auto& ghostPos : ghostsPos) {
				if (!isFloorBelow(ghostPos)) errorMessages += "Error: Ghost isn't standing on a floor, please insert a valid floor character at position: (" + std::to_string(ghostPos.getX()) + ", " + std::to_string(ghostPos.getY() + 1) + ")\n", isValid = false;
			}
		}

		if (isBounded) {
			fillBoardBoundaries(lastRow);
		}
	}

	minX += isBounded;
	minY += isBounded;
	maxX -= isBounded;
	*errors = errorMessages;
	return isValid;
}

/**
 * @brief Fills the remaining columns in a row with spaces if the line is shorter than the allowed width.
 * @param currCol The current column index.
 * @param currRow The current row index.
 */
void Board::fillMissingColumns(int currCol, int currRow) {
#pragma warning(suppress : 4996) 
	strcpy(originalBoard[currRow] + currCol, std::string(SCREEN_BOUNDARIES::MAX_X - currCol, ' ').c_str());
}

/**
 * @brief Fills the boundaries of the board with a specified character ('Q') to indicate borders.
 * @param lastRow The index of the last row in the board.
 */
void Board::fillBoardBoundaries(int lastRow) {
	memset(originalBoard[0], 'Q', SCREEN_BOUNDARIES::MAX_X);
	memset(originalBoard[lastRow], 'Q', SCREEN_BOUNDARIES::MAX_X);
	for (int row = 1; row < lastRow; ++row) {
		originalBoard[row][0] = originalBoard[row][SCREEN_BOUNDARIES::MAX_X - 1] = 'Q';
	}
}

/**
 * @brief Processes and validates the placement of a specific entity on the board.
 * @param c The current character being processed.
 * @param currCol The current column index.
 * @param currRow The current row index.
 * @param hasEntity Reference to a flag indicating if the entity has already been placed (for unique entities).
 * @param entityPos Reference to the position of the entity (for unique entities).
 * @param isValid Reference to a flag indicating if the board is valid.
 * @param errorMessages Reference to a string storing error messages.
 * @param entityPosList Optional reference to a vector storing positions of multiple instances (e.g., ghosts).
 */

void Board::handleMario(char& c, int currCol, int currRow, bool& hasMario, Point& marioPos, bool& isValid, std::string& errorMessages) {
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

void Board::handlePaulina(char& c, int currCol, int currRow, bool& hasPaulina, Point& paulinaPos, bool& isValid, std::string& errorMessages) {
	if (hasPaulina) {
		errorMessages += "Error: Multiple Paulina positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
		isValid = false;
	}
	else {
		paulinaPos = { currCol, currRow };
		hasPaulina = true;
	}
}

void Board::handleDonkey(char& c, int currCol, int currRow, bool& hasDonkey, Point& donkeyPos, bool& isValid, std::string& errorMessages) {
	if (hasDonkey) {
		errorMessages += "Error: Multiple Donkey positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
		isValid = false;
	}
	else {
		donkeyPos = { currCol, currRow };
		hasDonkey = true;
	}
}

void Board::handleGhost(char& c, int currCol, int currRow, std::vector<Point>& ghostsPos) {
	ghostsPos.push_back({ currCol, currRow });
	c = ' ';
}

void Board::handleHammer(char& c, int currCol, int currRow, bool& hasHammer, Point& hammerPos, bool& isValid) {
	if (!hasHammer) {
		hammerPos = { currCol, currRow };
		hasHammer = true;
	}
	else {
		isValid = false;
	}
}

void Board::handleLegend(char& c, int currCol, int currRow, bool& hasLegend, Point& legendPos, bool& isValid, std::string& errorMessages) {
	if (hasLegend) {
		errorMessages += "Error: Multiple Legend positions at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
		isValid = false;
	}
	else {
		legendPos = { currCol, currRow };
		hasLegend = true;
	}
	c = ' ';
}

void Board::handleBorderCharacter(char& c, int currCol, int currRow, bool& isBounded, bool& isValid, std::string& errorMessages) {
	if (currCol == SCREEN_BOUNDARIES::MIN_X || currCol == SCREEN_BOUNDARIES::MAX_X - 1 || currRow == SCREEN_BOUNDARIES::MIN_Y || currRow == SCREEN_BOUNDARIES::MAX_Y - 1) {
		isBounded = true;
	}
	else {
		errorMessages += "Error: Border character in the middle of the screen at (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
		isValid = false;
	}
}

void Board::handleUnknownCharacter(char c, int currCol, int currRow, bool& isValid, std::string& errorMessages) {
	errorMessages += "Error: Unknown character '" + std::string(1, c) + "' at position (" + std::to_string(currCol) + ", " + std::to_string(currRow) + ")\n";
	isValid = false;
}


