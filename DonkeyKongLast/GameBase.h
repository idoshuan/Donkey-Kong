#pragma once

#include "Mario.h"
#include "Board.h"
#include "Entity.h"
#include "Barrel.h"
#include "Ghost.h"
#include "ClimbingGhost.h"
#include "GhostBase.h"
#include "Menu.h"
#include "Screen.h"
#include "GameConfig.h"
#include "Hammer.h"
#include "Steps.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <optional>
#include <memory>
/**
 * @brief The Game class manages the main game loop, state transitions,
 * and interactions between entities like Mario, barrels, and the board.
 * It handles input, updates gameplay mechanics, and ensures smooth progression
 * through different game states.
 */



// ------------------- Class Declaration -------------------
class GameBase {
protected:
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

    // ------------------- Constants -------------------

    // Mario-related constants
    static constexpr int marioMaxFallHeight = 5;
    static constexpr int initLives = 3;
    static constexpr int blinkIterations = 6;

    // Barrel-related constants
    static constexpr int barrelSpawnInterval = 4000;
    static constexpr int barrelInitialSpawnDelay = 3000;
    static constexpr int barrelMaxFallHeight = 8;
    static constexpr int barrelExplosionRadius = 2;

    // Animation constants
    static constexpr int stageFinishPoints = 100;
    const std::string stageFinishPointsString = "+100";
    static constexpr int ghostKillPoints = 20;
    const std::string ghostKillPointsString = "+20";
    static constexpr int barrelKillPoints = 15;
    const std::string barrelKillPointsString = "+15";

    // Typedefs for time-related operations
    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using time = std::chrono::time_point<clock>;

    // ------------------- Game State Variables -------------------
    bool isRunning = true;

    GameState gameState;
    Screen screen;
    Menu menu;
    std::vector<std::string> fileNames;

    Board board;
    Mario mario;
    std::optional<Hammer> hammer;
    std::vector<Barrel> barrels;
    std::vector<std::unique_ptr<GhostBase>> ghostContainer;
    Point leftBarrelPos;
    Point rightBarrelPos;

    Steps steps;
    size_t iteration = 0; 

    int currLevel = 0;
    int score = 0;
    int lives = initLives;
    bool firstBarrelSpawned = false;

    time lastBarrelTime;
    time gameStartTime;

    // ------------------- Private Functions -------------------

    // Game Loop
    void startGame();
    virtual void handleGameState() = 0;
    void updateGameLogic();

    // Input Handling
    virtual void checkForKeyPress() = 0;

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
    virtual void startNewStage() = 0;
    void handleGameWin();
    void handleGameOver();

    // File Management
    virtual void getBoardFileNames() = 0;

    // Animation
    void marioBlinkAnimation();
    void hammerHitAnimation();
    void scoreAnimation(const std::string& points);

};
