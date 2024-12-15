#pragma once
#include "Utils.h"
#include "Screen.h"

#include <iostream>
#include <string>
#include <conio.h>

enum class MenuAction {
	START_GAME = 1,
	SHOW_INSTRUCTIONS = 8,
	EXIT_GAME = 9
};

class Menu : Screen {

const char* menu[SCREEN_BOUNDARIES::MAX_Y] = {
	R"(    _____                                                              _____    )", //1
	R"(   ( ___ )                                                            ( ___ )   )", //2
	R"(    |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   |    )", //3
	R"(    |   |                                                              |   |    )", //4
	R"(    |   |   ____              _                _  __                   |   |    )", //5
	R"(    |   |  |  _ \  ___  _ __ | | _____ _   _  | |/ /___  _ __   __ _   |   |    )", //6
	R"(    |   |  | | | |/ _ \| '_ \| |/ / _ \ | | | | ' // _ \| '_ \ / _` |  |   |    )", //7
	R"(    |   |  | |_| | (_) | | | |   <  __/ |_| | | . \ (_) | | | | (_| |  |   |    )", //8
	R"(    |   |  |____/ \___/|_| |_|_|\_\___|\__, | |_|\_\___/|_| |_|\__, |  |   |    )", //9
	R"(    |   |                              |___/                   |___/   |   |    )", //10
	R"(    |   |                                                              |   |    )", //11
	R"(    |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|    )", //12
	R"(   (_____)                                                            (_____)   )", //13
	R"(                  _      ___ _            _      ___                            )", //14
	R"(                 / |    / __| |_ __ _ _ _| |_   / __|__ _ _ __  ___             )", //15
	R"(                 | | _  \__ \  _/ _` | '_|  _| | (_ / _` | '  \/ -_)            )", //16
	R"(                 | _(_) |___/\__\__,_|_| \__ |  \___\__,_|_|_|_\___|            )", //17
	R"(                  ___     ___         _               _   _                     )", //18
	R"(                 ( _ )   |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___        )", //19
	R"(                 / _ \_   | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<        )", //20
	R"(                 \___(_) |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/        )", //21
	R"(                  ___    ___     _ _                                            )", //22
	R"(                 / _ \  | __|_ _(_) |_                                          )", //23
	R"(                 \_, /  | _|\ \ / |  _|                                         )", //24  
	R"(                  /_(_) |___/_\_\_|\__|                                         )", //25
};
   
const char* instructions[SCREEN_BOUNDARIES::MAX_Y] = {
	R"(                  ___         _               _   _                             )", //1
	R"(                 |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___                )", //2
	R"(                  | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<                )", //3
	R"(                 |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/                )", //4
	"                                                                                ", //5
	"                                                                                ", //6
	"                                                                                ", //7
	"           Your mission: Save Princess Paulina ($)!                             ", //8
	"           But watch out for those pesky barrels!                               ", //9
	"           Donkey Kong will throw them at you to stop your heroic rescue.       ", //10
	"                                                                                ", //11
	"           Controls:                                                            ", //12
	"           W - Move Up                                                          ", //13
	"           X - Move Down                                                        ", //14
	"           A - Move Left                                                        ", //15
	"           D - Move Right                                                       ", //16
	"           S - Stay Put (take a breather!)                                      ", //17
	"           ESC - Pause the game (but Paulina still needs you!)                  ", //18
	"                                                                                ", //19
	"           Avoid the barrels, climb to the top, and                             ", //20
	"           be the hero Paulina needs. Good luck!                                ", //21
	"           Press any key to return to the menu....                              ", //22
	"                                                                                ", //23
	"                                                                                ", //24
	"                                                                                ", //25
};

public:
	void displayMenu();
	MenuAction getAction();
	void displayInstructions();
};


