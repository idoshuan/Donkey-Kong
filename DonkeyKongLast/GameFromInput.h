#pragma once
#include "GameBase.h"

class GameFromInput :public GameBase {
private:
    // Game logic constants
    static constexpr int keyPressIterations = 5;
    static constexpr int gameLoopSleep = 12;

    // Pause screen constants
    static constexpr int pauseMessageX = 26;
    static constexpr int pauseMessageY = 12;
    static constexpr int pauseMessageWidth = 28;
    static constexpr int pauseMessageHeight = 5;
    static constexpr int pauseMessageTitleOffset = 8;
    static constexpr int pauseMessageContinueOffset = 1;

    // Countdown screen constants
    static constexpr int countdownMessageTitleOffset = 6;
    static constexpr int countdownMessageCounterOffset = 12;
    
    int prevStagesScore = 0;
    bool isAlreadyPaused = false;

    std::string stepsFilename;
    std::string resultsFilename;

    void handleGameState() override;
    void updateGameLogic() override;
    void checkForKeyPress() override;
    void startNewStage() override;
    void getBoardFileNames() override;
    void displayLives() const override;
    void handleGameWin() override;
    void handleGameOver() override;

    // Input Handling
    void handleMenuState(MenuAction action);

    bool checkMarioDeath() override;
    bool checkMarioWon() override;


    // Pause
    void handlePause();
    void displayPauseScreen();
    void clearMessageInsideBorders();
    void clearEntirePauseScreen();
    void displayCountdown();

    // File Management
    bool tryLoadNextValidBoard();

public:
    GameFromInput() { 
        gameState = GameState::MENU;
        startGame(); 
    };
};

