#pragma once
#include "Mario.h"
#include "Board.h"
#include "Entity.h"
#include "Barrel.h"
#include "Menu.h"
#include "GameConfig.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>

// ------------------- Enums -------------------
const enum class GameState {
    MENU,
    START,
    PLAYING,
    PAUSED,
    GAME_OVER,
    WON
};

// ------------------- Class Declaration -------------------
class Game {
   
    // ------------------- Constants -------------------
    // Mario-related constants
    static constexpr Point marioInitPos = { 4,23 };
    static constexpr int marioMaxFallHeight = 5;
    static constexpr int initLives = 3;

    // Barrel-related constants
    static constexpr int numBarrels = 20;
    static constexpr int barrelSpawnInterval = 4000;
    static constexpr int firstBarrelSpawnDelay = 3000;
    static constexpr int barrelMaxFallHeight = 8;
    static constexpr int explosionRadius = 2;

    // Pause-related constants
    static constexpr int pauseMessageX = 25;
    static constexpr int pauseMessageY = 12;
    static constexpr int pauseMessageWidth = 28;
    static constexpr int pauseMessageHeight = 5;
    
    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using time = std::chrono::time_point<clock>;

    // ------------------- Game State Variables -------------------
    bool isRunning = true;
    GameState gameState = GameState::MENU;
    Menu menu;
    Board board;
    Mario mario{ marioInitPos, &board };
    Barrel barrelArr[numBarrels];
    int lives = initLives;
    bool isAlreadyPaused = false;
    int barrelCount = 0;
    bool firstBarrelSpawned = false;
    Point leftBarrelPos = { board.getDonkeyKong().getX() - 1, board.getDonkeyKong().getY() };
    Point rightBarrelPos = { board.getDonkeyKong().getX() + 1, board.getDonkeyKong().getY() };
    time lastBarrelTime;
    time gameStartTime;
    // ------------------- Private Game Loop Functions -------------------
    void handleGameState();
    void handleGameWin();
    void updateGameLogic();
    void resetGame();
    void handleGameOver();
    void displayLives() const;

    // ------------------- Private Menu-Related Functions -------------------
    void handleMenuState(MenuAction action);

    // ------------------- Private Mario-Related Functions -------------------
    void marioBlink();
    bool checkMarioDeath();
    bool checkMarioWon();
    bool checkMarioDeathFromBarrel();
    bool checkMarioDeathFromFall();

    // ------------------- Private Barrel-Related Functions -------------------
    void trySpawnBarrel();
    void spawnBarrel();
    bool shouldSpawnFirstBarrel(const time& now) const;
    bool canSpawnBarrel(const time& now) const;
    bool hasBarrelExploded(Barrel& barrel) const;
    bool shouldDeactivateBarrel(Barrel& barrel) const;
    void deactivateBarrels();
    void explodeBarrels();

    // Collision and explosion checks
    bool isDirectCollision(const Barrel& barrel) const;
    bool isMissedCollision(const Barrel& barrel) const;
    bool isExplosionFatal(const Barrel& barrel) const;
    bool isInExplosionRadius(const Barrel& barrel) const;

    // ------------------- Private Input Handling -------------------
    void checkForKeyPress();

    // ------------------- Private Pause-Related Functions -------------------
    void handlePause();
    void displayPauseScreen();
    void clearPauseScreen();

    // ------------------- Private Utility Functions -------------------
    void eraseCharacters();
    void moveCharacters();
    void drawCharacters();

public:
    void startGame();
};
