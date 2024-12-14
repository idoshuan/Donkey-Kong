#pragma once
#include "Utils.h"
#include <iostream>

class Screen {
public:
    struct SCREEN_BOUNDARIES {
        static constexpr int MAX_X = 80;
        static constexpr int MIN_X = 0;
        static constexpr int MAX_Y = 25;
        static constexpr int MIN_Y = 0;
    };

    void printScreen(const char** screen);
    void clearScreen() {
        system("cls");
    }
}