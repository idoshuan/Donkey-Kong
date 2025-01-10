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

void Menu::displayBoardFiles(std::vector<std::string>& fileNames) {
    clearScreen();
    for (int i = 0; i < fileNames.size(); i++) {
        std::cout << i+1 << ". " << fileNames[i] << std::endl;
    }
    _getch();
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
