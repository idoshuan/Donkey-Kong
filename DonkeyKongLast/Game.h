#pragma once
#include "Mario.h"
#include "Board.h"
#include "Point.h"
#include "Barrel.h"
#include "Menu.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>

// ------------------- Enums -------------------
enum class GameState {
    MENU,
    START,
    PLAYING,
    PAUSED,
    GAME_OVER,
    WON
};

// ------------------- Class Declaration -------------------
class Game {
private:
    // ------------------- Constants -------------------
    // Mario-related constants
    static constexpr int marioInitX = 20;
    static constexpr int marioInitY = 0;
    static constexpr int marioMaxFallHeight = 5;
    static constexpr int initLives = 3;

    // Barrel-related constants
    static constexpr int numBarrels = 10;
    static constexpr int leftBarrelInitX = 38;
    static constexpr int rightBarrelInitX = 40;
    static constexpr int barrelInitY = 2;
    static constexpr int barrelSpawnInterval = 5000;
    static constexpr int firstBarrelSpawnDelay = 4000;
    static constexpr int barrelMaxFallHeight = 8;

    // General constants
    const int explosionRadius = 2;
    static constexpr int ESC = 27;

    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using time = std::chrono::time_point<clock>;

    // ------------------- Game State Variables -------------------
    GameState gameState;
    Menu menu;
    Mario mario;
    Board board;
    Barrel barrelArr[numBarrels];
    int lives;
    int barrelCount;
    bool firstBarrelSpawned;
    time lastBarrelTime;
    time gameStartTime;

    // ------------------- Private Game Loop Functions -------------------
    void handleGameState();
    void handleGameWin();
    void updateGameLogic();
    void resetGame();
    void handleGameOver() {
        return;
    }

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

    // Collision and explosion checks
    bool isDirectCollision(const Barrel& barrel) const;
    bool isMissedCollision(const Barrel& barrel) const;
    bool isExplosionFatal(const Barrel& barrel) const;
    bool isInExplosionRadius(const Barrel& barrel) const;

    // ------------------- Private Input Handling -------------------
    void checkForKeyPress();

    // ------------------- Private Pause-Related Functions -------------------
    void displayPauseScreen();
    void handlePauseInput();

    // ------------------- Private Utility Functions -------------------
    void eraseCharacters();
    void moveCharacters();
    void drawCharacters();

public:
    // ------------------- Constructor -------------------
    Game();

    // ------------------- Public Main Game Functions -------------------
    void startGame();
    
    
};
