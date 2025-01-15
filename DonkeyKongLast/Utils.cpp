#include <iostream>
#include "Utils.h"

/**
 * @brief Moves the console cursor to a specific position.
 * @param x The X-coordinate (horizontal position).
 * @param y The Y-coordinate (vertical position).
 */
void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;       // Create a COORD structure 
    coord.X = x;       // Set the X coordinate 
    coord.Y = y;       // Set the Y coordinate 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}

/**
 * @brief Toggles the visibility of the console cursor.
 * @param showFlag Set to true to show the cursor, false to hide it.
 */
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

/**
 * @brief Generate random double between min and max
 * @param min default to 0.0
 * @param max default to 1.0
 * Note: Created with the help of ChatGPT.
 */
double getRandomDouble(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}


