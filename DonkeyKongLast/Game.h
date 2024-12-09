#pragma once
#include "Mario.h"
#include "Board.h"
#include "Point.h"
#include "Barrel.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>

class Game {
private:
    // Mario-related constants
    static constexpr int marioInitX = 4;
    static constexpr int marioInitY = 23;
    static constexpr int marioMaxFallHeight = 5;
    static constexpr int initLives = 3;

    // Barrel-related constants
    static constexpr int numBarrels = 10;
    static constexpr int leftBarrelInitX = 37;
    static constexpr int rightBarrelInitX = 39;
    static constexpr int barrelInitY = 2;
    static constexpr int barrelSpawnInterval = 5000;
    static constexpr int firstBarrelSpawnDelay = 4000;
    static constexpr int barrelMaxFallHeight = 8;
    const int explosionRadius = 2;

    // General constants
    static constexpr int ESC = 27;
    using clock = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using time = std::chrono::time_point<clock>;

    // Game entities and state
    Mario mario;
    Board board;
    Barrel barrelArr[numBarrels];
    int lives;
    int barrelCount;
    bool firstBarrelSpawned;
    time lastBarrelTime;
    time gameStartTime;

    // Helper functions
    bool checkMarioDeathFromBarrel();
    bool checkMarioDeathFromFall();
    bool isInExplosionRadius(const Barrel& barrel) const;
    bool hasBarrelExploded(Barrel& barrel) const;
    bool shouldDeactivateBarrel(Barrel& barrel) const;
    bool isDirectCollision(const Barrel& barrel) const;
    bool isMissedCollision(const Barrel& barrel) const;
    bool isExplosionFatal(const Barrel& barrel) const;
    bool shouldSpawnFirstBarrel(const time& now) const;
    bool canSpawnBarrel(const time& now) const;
    void spawnBarrel();

public:
    // Constructor
    Game();

    // Main game functions
    void startGame();
    void resetGame();

    // Mario-related functions
    void marioBlink();
    bool checkMarioDeath();

    // Barrel-related functions
    void trySpawnBarrel();

    // Input handling
    void checkForKeyPress();

    // Utility
    void eraseCharacters();
    void drawAndMoveCharacters();
};
