#pragma once
#include "GameBase.h"

class GameFromInput : GameBase {
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

    bool isAlreadyPaused = false;

    std::string stepsFilename;

    void handleGameState() override;
    void checkForKeyPress() override;
    void startNewStage() override;
    void getBoardFileNames() override;

    // Input Handling
    void handleMenuState(MenuAction action);

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

