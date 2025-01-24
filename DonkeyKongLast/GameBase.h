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
#include "Results.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
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
	static constexpr int barrelMaxFallHeight = 8;
	static constexpr int barrelExplosionRadius = 2;

	// Animation constants
	static constexpr int stageFinishPoints = 100;
	const std::string stageFinishPointsString = "+100";
	static constexpr int ghostKillPoints = 20;
	const std::string ghostKillPointsString = "+20";
	static constexpr int barrelKillPoints = 15;
	const std::string barrelKillPointsString = "+15";

	// ------------------- Game State Variables -------------------

	bool isRunning = true;
	int currLevel = 0;
	int score = 0;
	int lives = initLives;

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

	Results results;
	Steps steps;
	size_t iteration = 0;

	
	// ------------------- Private Functions -------------------

	// Game Loop
	void startGame();
	virtual void handleGameState() = 0;
	virtual void updateGameLogic() = 0;

	// Input Handling
	virtual void checkForKeyPress() = 0;

	// Mario
	virtual bool checkMarioDeath() = 0;
	virtual bool checkMarioWon() = 0;
	bool checkMarioDeathFromBarrel();
	bool checkMarioDeathFromFall();
	bool checkMarioDeathFromGhost();

	// Barrel
	void trySpawnBarrel();
	void spawnBarrel();
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
	virtual void displayLives() const = 0;
	void eraseCharacters();
	void moveCharacters();
	void drawCharacters();
	void displayScore() const;

	// Stage 
	virtual void handleGameWin() = 0;
	virtual void handleGameOver() = 0;
	virtual void startNewStage() = 0;
	void resetStage();

	// File Management
	virtual void getBoardFileNames() = 0;

	// Animation
	void marioBlinkAnimation();
	void hammerHitAnimation();
	void scoreAnimation(const std::string& points);

};
