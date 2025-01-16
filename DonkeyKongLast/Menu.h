#pragma once
#include "Utils.h"
#include "Screen.h"
#include "GameConfig.h"

#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <cctype>
#include <iomanip>
#include <algorithm>


/**
 * @brief The Menu class handles the game's main menu interface,
 * allowing the player to start the game, view instructions, or exit.
 * It manages user input to determine the selected action.
 */

enum class MenuAction {
	START_GAME = 1,
	SHOW_BOARD_FILES = 2,
	SHOW_INSTRUCTIONS = 8,
	EXIT_GAME = 9
};

// ------------------- Class Declaration -------------------
class Menu : Screen {
	static constexpr size_t filesPerPage = 9;
public:
	void displayMenu();
	MenuAction getAction();
	void displayInstructions();
	void displayBoardFiles(std::vector<std::string>& fileNames, size_t totalFiles, size_t totalPages, size_t page);
	int getBoardChoice(std::vector<std::string>& fileNames);
	void printUnknownKeyMessage();
};


