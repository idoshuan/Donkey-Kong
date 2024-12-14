#include "Screen.h"

void Screen::printScreen(const char** screen) {
    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < 80; ++j) {
            std::cout << screen[i][j];
        }
        std::cout << '\n';
    }
}