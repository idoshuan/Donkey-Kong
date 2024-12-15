#include "Menu.h"

void Menu::displayMenu() {
	clearScreen(); 
	printScreen(menu);
}

void Menu::displayInstructions() {
	clearScreen();
	printScreen(instructions);
	_getch();
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
		return getAction();
	}
}
