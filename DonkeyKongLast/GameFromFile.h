#pragma once
#include <iostream>
#include <vector>

#include "GameBase.h"
 

// ------------------- Class Declaration -------------------
class GameFromFile : GameBase {
private:
    
    std::vector<std::string> stepsFileNames;

    // ------------------- Private Functions -------------------
    void handleGameState() override;
    void checkForKeyPress() override;
    void startNewStage() override;
    void getBoardFileNames() override;

    // File Management
    void LoadNextBoard();

public:
    GameFromFile() {
        gameState = GameState::START;
        startGame();
    }
};
