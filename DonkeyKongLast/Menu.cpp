#include "Menu.h"

void Menu::displayMenu() {
	clearScreen(); // Call system-specific clear screen (e.g., system("clear") or system("cls"))
	std::cout << "===========================\n";
	std::cout << "     Donkey Kong Game      \n";
	std::cout << "===========================\n";
	std::cout << "1. Start Game\n";
	std::cout << "8. Instructions\n";
	std::cout << "9. Exit\n";
	std::cout << "===========================\n";
}

MenuAction Menu::getAction() {
	char choice = _getch();

	switch (choice) {
	case '1':
		return MenuAction::START_GAME;
		break;
	case '8':
		return MenuAction::SHOW_INSTRUCTIONS;
		break;
	case '9':
		return MenuAction::EXIT_GAME;
		break;
	default:
		std::cout << "Invalid choice. Please try again.\n";
		return getAction();
	}
}

void Menu::displayInstructions() {
	clearScreen();
	std::cout << "===========================\n";
	std::cout << "       Instructions        \n";
	std::cout << "===========================\n";
	std::cout << "Use arrow keys to move.\n";
	std::cout << "Avoid obstacles and reach the top.\n";
	std::cout << "Good luck!\n";
	std::cout << "===========================\n";
	std::cout << "Press Enter to return to the menu.\n";
	_getch();
}