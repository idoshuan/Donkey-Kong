#include "GameBase.h"
#include "GameFromFile.h"
#include "GameFromInput.h"
#include <iostream>

/**
 * @brief Entry point of the program. Handles game mode and arguments.
 */
int main(int argc, const char** argv) {
    // Determine if the game is in load mode and silent mode
    bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
    bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";

    GameBase* game;  // Pointer to the base game object

    // Create game instance based on mode
    if (isLoad) {
        GameFromFile a = GameFromFile(isSilent);  // Load game from file
        game = &a;
    }
    else {
        GameFromInput b = GameFromInput();  // Start game from user input
        game = &b;
    }
}
