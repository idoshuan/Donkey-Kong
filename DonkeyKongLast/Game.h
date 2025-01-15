#pragma once

#include "Mario.h"
#include "Board.h"
#include "Entity.h"
#include "Barrel.h"
#include "Ghost.h"
#include "Menu.h"
#include "Screen.h"
#include "GameConfig.h"
#include "Hammer.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <optional>

/**
 * @brief The Game class manages the main game loop, state transitions,
 * and interactions between entities like Mario, barrels, and the board.
 * It handles input, updates gameplay mechanics, and ensures smooth progression
 * through different game states.
 */

 // ------------------- Enums -------------------
enum class GameState {
    MENU,
    START,
    PLAYING,
    PAUSED,
    LEVEL_WON,
    GAME_OVER,
    WON
};

// ------------------- Class Declaration -------------------
class Game {
private:
    // ------------------- Constants -------------------
    // Mario-related constants
    static constexpr int marioMaxFallHeight = 5;
    static constexpr int initLives = 3;
    static constexpr int blinkIterations = 6;

    // Barrel-related constants
    static constexpr int maxBarrels = 20;
    static constexpr int barrelSpawnInterval = 4000;
    static constexpr int barrelInitialSpawnDelay = 3000;
    static constexpr int barrelMaxFallHeight = 8;
    static constexpr int barrelExplosionRadius = 2;

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

    // Typedefs for time-related operations
    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using time = std::chrono::time_point<clock>;

    // ------------------- Game State Variables -------------------
    bool isRunning = true;
    GameState gameState = GameState::MENU;
    std::vector<std::string> fileNames;
    Screen screen;
    Menu menu;
    Board board;
    Mario mario;
    std::optional<Hammer> hammer;
    std::vector<Barrel> barrels;
    std::vector<Ghost> ghosts;

    Point leftBarrelPos;
    Point rightBarrelPos;

    int currLevel = 0;
    int score = 0;
    int lives = initLives;
    bool isAlreadyPaused = false;
    bool firstBarrelSpawned = false;

    time lastBarrelTime;
    time gameStartTime;

    // ------------------- Private Functions -------------------

    // Game Loop
    void startGame();
    void handleGameState();
    void updateGameLogic();

    // Input Handling
    void checkForKeyPress();
    void handleMenuState(MenuAction action);

    // Mario
    bool checkMarioDeath();
    bool checkMarioWon();
    bool checkMarioDeathFromBarrel();
    bool checkMarioDeathFromFall();
    bool checkMarioDeathFromGhost();

    // Barrel
    void trySpawnBarrel();
    void spawnBarrel();
    bool shouldSpawnFirstBarrel(const time& now) const;
    bool canSpawnBarrel(const time& now) const;
    bool hasBarrelExploded(Barrel& barrel) const;
    bool shouldDeactivateBarrel(Barrel& barrel) const;
    void deactivateBarrels();
    void explodeBarrels();

    // Ghost
    void checkGhostWithGhostCollisions();

    // Hammer
    void checkHammerPickUp();
    void checkKill();

    // Collision
    bool isDirectCollision(const Entity& entity) const;
    bool isMissedCollision(const Entity& entity) const;
    bool isExplosionFatal(const Barrel& barrel) const;
    bool isInExplosionRadius(const Barrel& barrel) const;

    // Utility
    void eraseCharacters();
    void moveCharacters();
    void drawCharacters();
    void displayLives() const;
    void displayScore() const;

    // Stage 
    void resetStage();
    void startNewStage();
    void handleGameWin();
    void handleGameOver();

    // Pause
    void handlePause();
    void displayPauseScreen();
    void clearMessageInsideBorders();
    void clearEntirePauseScreen();
    void displayCountdown();

    // File Management
    void getBoardFileNames(std::vector<std::string>& fileNames);
    bool tryLoadNextValidBoard();

    // Animation
    void marioBlinkAnimation();
    void hammerHitAnimation();
    void scoreAnimation(const std::string& points);

public:
    Game() {
        startGame();
    }
};
