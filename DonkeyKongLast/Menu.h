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

const char* menu[25] = {
	"    _____                                                              _____    ", //1
	"   ( ___ )                                                            ( ___ )   ", //2
	"    |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   |    ", //3
	"    |   |                                                              |   |    ", //4
	"    |   |   ____              _                _  __                   |   |    ", //5
	"    |   |  |  _ \  ___  _ __ | | _____ _   _  | |/ /___  _ __   __ _   |   |    ", //6
	"    |   |  | | | |/ _ \| '_ \| |/ / _ \ | | | | ' // _ \| '_ \ / _` |  |   |    ", //7
	"    |   |  | |_| | (_) | | | |   <  __/ |_| | | . \ (_) | | | | (_| |  |   |    ", //8
	"    |   |  |____/ \___/|_| |_|_|\_\___|\__, | |_|\_\___/|_| |_|\__, |  |   |    ", //9
	"    |   |                              |___/                   |___/   |   |    ", //10
	"    |   |                                                              |   |    ", //11
	"    |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|    ", //12
	"   (_____)                                                            (_____)   ", //13
	"                   _      ___ _            _      ___                           ", //14
	"                  / |    / __| |_ __ _ _ _| |_   / __|__ _ _ __  ___            ", //15
	"                  | | _  \__ \  _/ _` | '_|  _| | (_ / _` | '  \/ -_)           ", //16
	"                  | _(_) |___/\__\__,_|_| \__ |  \___\__,_|_|_|_\___|           ", //17
	"                   ___     ___         _               _   _                    ", //18
	"                  ( _ )   |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___       ", //19
	"                  / _ \_   | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<       ", //20
	"                  \___(_) |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/       ", //21
	"                   ___    ___     _ _                                           ", //22
	"                  / _ \  | __|_ _(_) |_                                         ", //23
	"                  \_, /  | _|\ \ / |  _|                                        ", //24  
	"                   /_(_) |___/_\_\_|\__|                                        ", //25
};
   
const char* instructions[25] = {
	"                  ___         _               _   _                             ", //1
	"                 |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___                ", //2
	"                  | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<                ", //3
	"                 |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/                ", //4
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


