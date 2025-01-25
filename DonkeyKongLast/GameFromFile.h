#pragma once
#include <iostream>
#include <vector>
#include <sstream> 

#include "GameBase.h"


// ------------------- Class Declaration -------------------
class GameFromFile :public GameBase {
private:

	// ------------------- Constants -------------------
	
	// Load/Silent constants
	static constexpr int sleepForLoad = 30;
	static constexpr int sleepForSilent = 0;
	bool isSilent;

	// ------------------- Game Files -------------------

	std::vector<std::string> stepsFileNames;
	std::vector<std::string> resultsFileNames;

	// ------------------- Private Functions -------------------
	
	// Game Loop
	void handleGameState() override;
	void updateGameLogic() override;

	// Input Handling
	void checkForKeyPress() override;

	// Mario
	bool checkMarioDeath() override;
	bool checkMarioWon() override;

	// Utility
	void displayLives() const override;
	
	// Stage 
	void startNewStage() override;
	void handleGameWin() override;
	void handleGameOver() override;

	// File Management
	void LoadNextBoard();
	void getBoardFileNames() override;

public:
	GameFromFile(bool silent = false) :isSilent(silent) {
		gameState = GameState::START;
		startGame();
	}
};
