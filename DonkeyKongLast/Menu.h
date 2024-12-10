#pragma once
#include "Utils.h"

#include <iostream>
#include <string>
#include <conio.h>

enum class MenuAction {
	START_GAME = 1,
	SHOW_INSTRUCTIONS = 8,
	EXIT_GAME = 9
};

class Menu {
public:
	void displayMenu();
	MenuAction getAction();
	void displayInstructions();
};
