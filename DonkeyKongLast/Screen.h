#pragma once
#include "Utils.h"
#include <iostream>

// ------------------- Class Declaration -------------------
class Screen {
public:
    struct SCREEN_BOUNDARIES {
        static constexpr int MAX_X = 80;
        static constexpr int MIN_X = 0;
        static constexpr int MAX_Y = 25;
        static constexpr int MIN_Y = 0;
    };

private:
    // ------------------- Static Screens -------------------
    static const char* menuScreen[SCREEN_BOUNDARIES::MAX_Y];
    static const char* instructionsScreen[SCREEN_BOUNDARIES::MAX_Y];
    static const char* winScreen[SCREEN_BOUNDARIES::MAX_Y];
    static const char* loseScreen[SCREEN_BOUNDARIES::MAX_Y];

    // ------------------- Private Functions -------------------
    void printScreen(const char** screen, int score = -1) const;

public:
    // ------------------- Public Functions -------------------
    void clearScreen() const {
        system("cls");
    }
    void printMenuScreen() const {
        printScreen(menuScreen);
    }
    void printInstructionsScreen() const {
        printScreen(instructionsScreen);
    }
    void printWinScreen(int score) const{
        printScreen(winScreen, score);
    }
    void printLoseScreen(int score) const{
        printScreen(loseScreen, score);
    }
};