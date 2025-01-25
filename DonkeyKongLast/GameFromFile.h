#pragma once
#include <iostream>
#include <vector>
#include <sstream> 

#include "GameBase.h"


// ------------------- Class Declaration -------------------
class GameFromFile :public GameBase {
private:

	std::vector<std::string> stepsFileNames;
	std::vector<std::string> resultsFileNames;

	bool isSilent;
	// ------------------- Private Functions -------------------
	void handleGameState() override;
	void updateGameLogic() override;
	void checkForKeyPress() override;
	bool checkMarioDeath() override;
	bool checkMarioWon() override;
	void startNewStage() override;
	void getBoardFileNames() override;
	void handleGameWin() override;
	void handleGameOver() override;

	// File Management
	void LoadNextBoard();
	void displayLives() const override;

public:
	GameFromFile(bool silent = false) :isSilent(silent) {
		gameState = GameState::START;
		startGame();
	}
};
