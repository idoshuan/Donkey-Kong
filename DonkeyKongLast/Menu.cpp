#include "Menu.h"

// ------------------- Public Functions -------------------

/**
 * @brief Displays the main menu screen.
 * Clears the screen before displaying the menu.
 */
void Menu::displayMenu() {
	clearScreen();
	printMenuScreen();
}

/**
 * @brief Displays the instructions screen.
 * Clears the screen before showing instructions and waits for user input to return.
 */
void Menu::displayInstructions() {
	clearScreen();
	printInstructionsScreen();
	_getch(); // Wait for user input
}



/**
 * @brief Gets the player's selected action from the menu.
 * Prompts the user to press a key corresponding to a menu option.
 * @return The selected MenuAction.
 */
MenuAction Menu::getAction() {
	char choice = _getch();

	switch (choice) {
	case '1':
		return MenuAction::START_GAME;
	case '2':
		return MenuAction::SHOW_BOARD_FILES;
	case '8':
		return MenuAction::SHOW_INSTRUCTIONS;
	case '9':
		return MenuAction::EXIT_GAME;
	default:
		return getAction();
	}
}


int Menu::getBoardChoice(std::vector<std::string>& fileNames) {
	size_t totalFiles = fileNames.size();
	size_t totalPages = (totalFiles + filesPerPage - 1) / filesPerPage;
	size_t page = 0;
	bool refreshDisplay = true; 

	while (true) {
		
		if (refreshDisplay) {
			displayBoardFiles(fileNames, totalFiles, totalPages, page);
		}

		char choice = _getch();
		refreshDisplay = true;

		if (choice == -32 || choice == 0) { // Arrow keys start with -32 or 0
			choice = _getch(); // Get the actual arrow key code
		}

		if (std::isdigit(choice)) {
			int c = choice - '0' - 1;
			int index = page * 9 + c;
			if (c >= 0 && c < 9 && index < fileNames.size()) {
				return index;
			}
			else {
				refreshDisplay = false;
			}
		}
		else if (choice == KEYS::NEXT) {
			page < totalPages - 1 ? ++page : refreshDisplay = false;
		}
		else if (choice == KEYS::PREV) {
			page > 0 ? --page : refreshDisplay = false;
		}
		else if (choice == KEYS::ESC) {
			return -1;
		}
		else {
			printUnknownKeyMessage();
			refreshDisplay = false;
		}
	}
}


void Menu::displayBoardFiles(std::vector<std::string>& fileNames, size_t totalFiles, size_t totalPages, size_t page) {
	clearScreen();

	size_t startIndex = page * filesPerPage;
	size_t endIndex = (startIndex + filesPerPage < totalFiles) ? (startIndex + filesPerPage) : totalFiles;

	std::string title = "Your Boards:";
	size_t titlePadding = (SCREEN_BOUNDARIES::MAX_X - title.length()) / 2;
	size_t filesPadding = (SCREEN_BOUNDARIES::MAX_X - fileNames[startIndex].length()) / 2;

	// Display header
	std::cout << "\n\n" << std::string(titlePadding, ' ') << title << "\n\n\n";
	std::cout << std::string(SCREEN_BOUNDARIES::MAX_X, '=') << "\n\n";

	// Display files for the current page
	for (size_t i = startIndex; i < endIndex; ++i) {
		std::cout << std::string(filesPadding, ' ');
		std::cout << std::to_string(i - startIndex + 1) << ". " << fileNames[i] << '\n';
	}

	// Print centered "Page X of Y"
	gotoxy(0, 18);
	std::string pageInfo = "Page " + std::to_string(page + 1) + " of " + std::to_string(totalPages);
	size_t pagePadding = (SCREEN_BOUNDARIES::MAX_X - pageInfo.length()) / 2;
	std::cout << "\n" << std::string(pagePadding, ' ') << pageInfo << "\n\n";

	// Prepare "Next Page" and "Previous Page"
	std::string nextPage = (page != totalPages - 1) ? "Next Page ->" : "";
	std::string prevPage = (page != 0) ? "<- Previous Page" : "";

	// Print navigation options
	std::cout << std::setw(0) << std::left << prevPage;
	std::cout << std::setw(SCREEN_BOUNDARIES::MAX_X - prevPage.length() - nextPage.length()) << " "; // Fill space in between
	std::cout << std::right << nextPage << "\n";

	// Print "Menu - ESC" centered below
	std::string menuText = "Menu - ESC";
	size_t menuPadding = (SCREEN_BOUNDARIES::MAX_X - menuText.length()) / 2;
	std::cout << std::string(menuPadding, ' ') << menuText << "\n";
}

void Menu::printUnknownKeyMessage() {
	gotoxy(0, 24);
	std::cout << "Invalid input. Use arrow keys or select 1-9.";
	Sleep(1500);
	gotoxy(0, 24);
	std::cout << std::string(80, ' ');
}

