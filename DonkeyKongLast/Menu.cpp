#include "Menu.h"

void Menu::displayMenu() {
	clearScreen(); 
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
	
	_getch();
}