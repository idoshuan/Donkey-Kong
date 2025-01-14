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
    constexpr int columnWidth = SCREEN_BOUNDARIES::MAX_X / 2; 
    const int rows = (fileNames.size() + 1) / 2; 

    std::string title = "Available Files :";
    int titlePadding = (SCREEN_BOUNDARIES::MAX_X - title.length()) / 2;
    std::cout << std::string(titlePadding, ' ') << title << "\n\n\n";
    std::cout << std::string(SCREEN_BOUNDARIES::MAX_X, '=') << "\n\n";

    for (int row = 0; row < rows; ++row) {
        // Left column
        int leftIndex = row;
        if (leftIndex < fileNames.size()) {
            std::cout << std::setw(columnWidth - 2) << std::left
                << (std::to_string(leftIndex + 1) + ". " + fileNames[leftIndex]);
        }
        else {
            std::cout << std::setw(columnWidth - 2) << " "; 
        }

        // Right column
        int rightIndex = row + rows;
        if (rightIndex < fileNames.size()) {
            std::cout << std::setw(columnWidth - 2) << std::left
                << (std::to_string(rightIndex + 1) + ". " + fileNames[rightIndex]);
        }
        else {
            std::cout << std::setw(columnWidth - 2) << " "; 
        }

        std::cout << '\n'; 
    }

    std::cout << "\n" << std::string(SCREEN_BOUNDARIES::MAX_X, '=') << "\n";
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

int Menu::getBoardChoice(size_t filesSize) {
    while (true) { // Loop until a valid input is received
        char choice = _getch();

        if (std::isdigit(choice)) {
            int c = choice - '0' - 1;
            if (c < filesSize && c >= 0) {
                return c; // Valid input
            }
        }

        gotoxy(0, 24);
        std::cout << "Select number between 1 and " << filesSize << ".";
        Sleep(2000);

        gotoxy(0, 24);
        std::cout << std::string(80, ' ');
    }
}
