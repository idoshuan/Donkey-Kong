#pragma once
#include <iostream>
#include <vector>
#include <sstream> 

#include "GameBase.h"
#include "Results.h"

/**
 * @brief Loads a game state from a file, reconstructing the board,
 * player positions, and other necessary data from a saved file.
 */

// ------------------- Class Declaration -------------------
class GameFromFile :public GameBase {
private:

	// ------------------- Constants -------------------
	
	// Load/Silent constants
	static constexpr int sleepForLoad = 10;
	static constexpr int sleepForSilent = 0;

	// ------------------- Game Files -------------------

	std::vector<std::string> stepsFileNames;
	std::vector<std::string> resultsFileNames;

	// ------------------- Private Functions -------------------
	
	// Game Loop
	void handleGameState() override;

	// Input Handling
	void checkForKeyPress() override;

	// Mario
	bool checkMarioDeath() override;
	bool checkMarioWon() override;

	
	// Stage 
	void startNewStage() override;
	void handleGameWin() override;
	void handleGameOver() override;

	// File Management
	void LoadNextBoard();
	void getBoardFileNames() override;

public:
	GameFromFile(bool silent = false) : GameBase(silent) {
		gameState = GameState::START;
	}
};
