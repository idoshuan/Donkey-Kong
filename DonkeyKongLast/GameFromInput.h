#pragma once
#include "GameBase.h"

/**
 * @brief Starts a new game by taking input from the user to configure
 * the initial game state, such as board setup and entity placement.
 */

class GameFromInput :public GameBase {

private:
	// ------------------- Constants -------------------

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

	// ------------------- Game State Variables -------------------

	int prevStagesScore = 0;
	bool isAlreadyPaused = false;

	std::string stepsFilename;
	std::string resultsFilename;

	// ------------------- Private Functions -------------------

	// Game Loop
	void handleGameState() override;

	// Input Handling
	void handleMenuState(MenuAction action);
	void checkForKeyPress() override;

	// Mario
	bool checkMarioDeath() override;
	bool checkMarioWon() override;

	// Stage 
	void startNewStage() override;
	void handleGameWin() override;
	void handleGameOver() override;

	// File Management
	void getBoardFileNames() override;
	bool tryLoadNextValidBoard();
	void saveStepsAndResults();

	// Pause
	void handlePause();
	void displayPauseScreen();
	void clearMessageInsideBorders();
	void clearEntirePauseScreen();
	void displayCountdown();

	bool save = false;

public:
	GameFromInput(bool save = false) :save(save) {
		gameState = GameState::MENU;
	};
};

