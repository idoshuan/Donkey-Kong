#include "Screen.h"

// ------------------- Public Functions -------------------

/**
 * @brief Prints a screen using a 2D array of strings.
 * Clears the console screen first, then displays the provided screen array.
 *
 * @param screen A 2D array of strings representing the screen to print.
 */
void Screen::printScreen(const char** screen) const {
    clearScreen();

    for (int x = 0; x < SCREEN_BOUNDARIES::MAX_X; ++x) {
        for (int y = 0; y < SCREEN_BOUNDARIES::MAX_Y; ++y) {
            gotoxy(x,y);
            std::cout << screen[y][x]; // Access character at row `y`, column `x`
        }
        Sleep(1);         // Delay for effect
    }
    eatBuffer();
}



// ------------------- Static Screens -------------------

// Menu Screen
/**
 * @brief ASCII art for the main menu screen.
 * Provides options for starting the game, viewing instructions, or exiting.
 */
const char* Screen::menuScreen[SCREEN_BOUNDARIES::MAX_Y] = {
    R"(    _____                                                              _____    )",
    R"(   ( ___ )                                                            ( ___ )   )",
    R"(    |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   |    )",
    R"(    |   |                                                              |   |    )",
    R"(    |   |   ____              _                _  __                   |   |    )",
    R"(    |   |  |  _ \  ___  _ __ | | _____ _   _  | |/ /___  _ __   __ _   |   |    )",
    R"(    |   |  | | | |/ _ \| '_ \| |/ / _ \ | | | | ' // _ \| '_ \ / _` |  |   |    )",
    R"(    |   |  | |_| | (_) | | | |   <  __/ |_| | | . \ (_) | | | | (_| |  |   |    )",
    R"(    |   |  |____/ \___/|_| |_|_|\_\___|\__, | |_|\_\___/|_| |_|\__, |  |   |    )",
    R"(    |   |                              |___/                   |___/   |   |    )",
    R"(    |   |                                                              |   |    )",
    R"(    |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___|    )",
    R"(   (_____)                                                            (_____)   )",
    R"(                                                                                )",
    R"(                                                                                )",
    R"(                                                                                )",
    R"(                               1. START GAME                                    )",
    R"(                                                                                )",
    R"(                               2. CHOOSE BOARD FILE                             )",
    R"(                                                                                )",
    R"(                               8. INSTRUCTIONS                                  )",
    R"(                                                                                )",
    R"(                               9. EXIT                                          )",
    R"(                                                                                )",
    R"(                                                                                )",
};

// Instructions Screen
/**
 * @brief ASCII art for the instructions screen.
 * Explains the game controls and the mission to rescue Paulina.
 */
const char* Screen::instructionsScreen[SCREEN_BOUNDARIES::MAX_Y] = {
    R"(                  ___         _               _   _                             )",
    R"(                 |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___                )",
    R"(                  | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<                )",
    R"(                 |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/                )",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
    "           Your mission: Save Princess Paulina ($)!                             ",
    "           But watch out for those pesky barrels!                               ",
    "           Donkey Kong will throw them at you to stop your heroic rescue.       ",
    "                                                                                ",
    "           Controls:                                                            ",
    "           W - Move Up                                                          ",
    "           X - Move Down                                                        ",
    "           A - Move Left                                                        ",
    "           D - Move Right                                                       ",
    "           S - Stay Put (take a breather!)                                      ",
    "           ESC - Pause the game (but Paulina still needs you!)                  ",
    "                                                                                ",
    "           Avoid the barrels, climb to the top, and                             ",
    "           be the hero Paulina needs. Good luck!                                ",
    "           Press any key to return to the menu....                              ",
    "                                                                                ",
    "                                                                                ",
    "                                                                                ",
};


// Instructions Screen
/**
 * @brief ASCII art for the win screen.
 */
const char* Screen::winScreen[SCREEN_BOUNDARIES::MAX_Y] = {
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(           .--..--..--..--..--..--..--..--..--..--..--..--..--..--.             )",
   R"(          / .. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \            )",
   R"(          \ \/\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ \/ /            )",
   R"(           \/ /`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\/ /             )",
   R"(           / /\                                                / /\             )",
   R"(          / /\ \   __   __           __        ___       _    / /\ \            )",
   R"(          \ \/ /   \ \ / /__  _   _  \ \      / (_)_ __ | |   \ \/ /            )",
   R"(           \/ /     \ V / _ \| | | |  \ \ /\ / /| | '_ \| |    \/ /             )",
   R"(           / /\      | | (_) | |_| |   \ V  V / | | | | |_|    / /\             )",
   R"(          / /\ \     |_|\___/ \__,_|    \_/\_/  |_|_| |_(_)   / /\ \            )",
   R"(          \ \/ /                                              \ \/ /            )",
   R"(           \/ /                                                \/ /             )",
   R"(           / /\.--..--..--..--..--..--..--..--..--..--..--..--./ /\             )",
   R"(          / /\ \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \/\ \            )",
   R"(          \ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `' /  \/            )",
   R"(           `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'             )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )"
};

// Instructions Screen
/**
 * @brief ASCII art for the lose screen.
 */
const char* Screen::loseScreen[SCREEN_BOUNDARIES::MAX_Y] = {
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(           .--..--..--..--..--..--..--..--..--..--..--..--..--..--.             )",
   R"(          / .. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \            )",
   R"(          \ \/\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ \/ /            )",
   R"(           \/ /`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\/ /             )",
   R"(           / /\                                                / /\             )",
   R"(          / /\ \  __   __            _                   _    / /\ \            )",
   R"(          \ \/ /  \ \ / /__  _   _  | |    ___  ___  ___| |   \ \/ /            )",
   R"(           \/ /    \ V / _ \| | | | | |   / _ \/ __|/ _ \ |    \/ /             )",
   R"(           / /\     | | (_) | |_| | | |__| (_) \__ \  __/_|    / /\             )",
   R"(          / /\ \    |_|\___/ \__,_| |_____\___/|___/\___(_)   / /\ \            )",
   R"(          \ \/ /                                              \ \/ /            )",
   R"(           \/ /                                                \/ /             )",
   R"(           / /\.--..--..--..--..--..--..--..--..--..--..--..--./ /\             )",
   R"(          / /\ \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \/\ \            )",
   R"(          \ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `' /  \/            )",
   R"(           `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'             )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )",
   R"(                                                                                )"
};


