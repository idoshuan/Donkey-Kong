#include "GameFromInput.h"

// ------------------- Game Loop -------------------

/**
 * @brief Handles the logic for the current game state.
 * Switches between states like MENU, START, PLAYING, PAUSED, GAME_OVER, and WON.
 */
void GameFromInput::handleGameState() {
	switch (gameState) {
	case GameState::MENU:
		menu.displayMenu();
		handleMenuState(menu.getAction());
		break;
	case GameState::START:
		gameState = GameState::PLAYING;
		startNewStage();
		break;
	case GameState::PLAYING:
		if (lives > 0) {
			updateGameLogic();
		}
		else {
			gameState = GameState::GAME_OVER;
		}
		break;
	case GameState::PAUSED:
		handlePause();
		break;
	case GameState::LEVEL_WON:
		results.addResult(iteration, Results::finished);
		steps.saveSteps(stepsFilename);
		results.saveResults(resultsFilename, score - prevStagesScore);
		if (currLevel == fileNames.size()) {
			gameState = GameState::WON;
		}
		else {
			gameState = GameState::PLAYING;
			startNewStage();
		}
		break;
	case GameState::GAME_OVER:
		steps.saveSteps(stepsFilename);
		results.saveResults(resultsFilename, score - prevStagesScore);
		handleGameOver();
		break;
	case GameState::WON:
		handleGameWin();
		break;
	default:
		std::cerr << "Unknown game state!" << std::endl;
		isRunning = false;
	}
}




// ------------------- Input Handling -------------------

/**
 * @brief Checks for user input during gameplay.
 * Processes ESC to pause the game or sends key input to Mario.
 */
void GameFromInput::checkForKeyPress() {
	bool marioKeyPressed = false;
	bool hammerKeyPressed = false;

	for (int i = 0; i < keyPressIterations; i++) {
		Sleep(gameLoopSleep);
		if (_kbhit()) {
			KEYS key = charToKey(_getch());
			if (key != KEYS::INVALID) {
				if (key == KEYS::ESC) {
					gameState = GameState::PAUSED;
					return;
				}
				else if (key != KEYS::HAMMER && !marioKeyPressed) {
					marioKeyPressed = true;
					if(save)steps.addStep({ iteration, key });
					mario.keyPressed(key);
				}
				else if (hammer && key == KEYS::HAMMER && !hammerKeyPressed) {
					hammerKeyPressed = true;
					if(save)steps.addStep({ iteration, key });
					hammer->swing();
				}
			}
		}
	}
}




/**
 * @brief Starts a new stage by loading the next board.
 * Initializes Mario, ghosts, barrels, and updates the game display.
 */
void GameFromInput::startNewStage() {
	if (!tryLoadNextValidBoard()) {
		std::cout << "No additional valid board found, returning to menu";
		Sleep(2000);
		currLevel = 0;
		gameState = GameState::MENU;
		return;
	}

	if (hammer) {
		hammer.reset();
	}
	ghostContainer.clear();
	barrels.clear();

	mario = Mario(board);
	for (const auto& ghostPos : board.getGhostsPos()) {
		ghostContainer.push_back(std::make_unique<Ghost>(ghostPos, board));
	}
	for (const auto& ghostPos : board.getClimbingGhostsPos()) {
		ghostContainer.push_back(std::make_unique<ClimbingGhost>(ghostPos, board));
	}

	auto donkeyPos = board.getDonkeyKongPos();
	leftBarrelPos = { donkeyPos.getX() - 1, donkeyPos.getY() };
	rightBarrelPos = { donkeyPos.getX() + 1, donkeyPos.getY() };

	board.reset();
	board.print();
	displayLives();
	displayScore();
	prevStagesScore = score;
}


// ------------------- File Management Functions -------------------

/**
 * @brief Retrieves all valid game board files from the current directory.
 * Filters files starting with "dkong_" and ending with ".screen", then sorts them lexicographically.
 */
void GameFromInput::getBoardFileNames() {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 6) == "dkong_" && filename.extension() == ".screen") {
			fileNames.push_back(filenameStr);
		}
	}
	std::sort(fileNames.begin(), fileNames.end());
}

/**
 * @brief Handles menu actions like starting the game, showing instructions, or exiting.
 */
void GameFromInput::handleMenuState(MenuAction action) {
	switch (action) {
	case MenuAction::START_GAME:
		gameState = GameState::START;
		break;
	case MenuAction::SHOW_BOARD_FILES:
		currLevel = menu.getBoardChoice(fileNames);
		if (currLevel == ESCAPE) {
			currLevel = 0;
			gameState = GameState::MENU;
		}
		else {
			gameState = GameState::START;
		}
		break;
	case MenuAction::SHOW_INSTRUCTIONS:
		menu.displayInstructions();
		break;
	case MenuAction::EXIT_GAME:
		isRunning = false;
		break;
	}
}


/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool GameFromInput::checkMarioDeath() {
	if (checkMarioDeathFromBarrel() || checkMarioDeathFromFall() || checkMarioDeathFromGhost()) {
		results.addResult(iteration, Results::death);
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool GameFromInput::checkMarioWon() {
	return mario.getNextPos() == board.getPaulinaPos() || mario.getPos() == board.getPaulinaPos();
}

// ------------------- Pause Handling -------------------

/**
 * @brief Handles the pause state during gameplay.
 * Displays the pause screen and waits for ESC to resume.
 */
void GameFromInput::handlePause() {
	if (!isAlreadyPaused) {
		drawCharacters(); // Show characters while screen is paused
		displayPauseScreen();
		isAlreadyPaused = true;
	}

	if (_kbhit()) {
		char key = _getch();
		if (key == KEYS::ESC) {
			clearMessageInsideBorders();
			displayCountdown();
			clearEntirePauseScreen();
			isAlreadyPaused = false;
			gameState = GameState::PLAYING;
			eatBuffer();
		}
	}
}

/**
 * @brief Displays the pause screen with instructions to resume.
 */
void GameFromInput::displayPauseScreen() {
	gotoxy(pauseMessageX, pauseMessageY);
	std::cout << "---------------------------";
	gotoxy(pauseMessageX, pauseMessageY + 1);
	std::cout << "       GAME PAUSED         ";
	gotoxy(pauseMessageX + pauseMessageContinueOffset, pauseMessageY + 2);
	std::cout << "Press ESC again to resume";
	gotoxy(pauseMessageX, pauseMessageY + 3);
	std::cout << "---------------------------";
}

/**
 * @brief Clears only the message inside the borders.
 */
void GameFromInput::clearMessageInsideBorders() {
	for (int y = 1; y < pauseMessageHeight - 2; ++y) {
		gotoxy(pauseMessageX + pauseMessageContinueOffset, pauseMessageY + y);
		for (int x = 0; x < pauseMessageWidth - 2; ++x) {
			std::cout << ' '; // Clear inner content character by character
		}
	}
}

/**
 * @brief Clears the pause screen.
 */
void GameFromInput::clearEntirePauseScreen() {
	for (int y = 0; y < pauseMessageHeight; ++y) {
		gotoxy(pauseMessageX, pauseMessageY + y);
		for (int x = 0; x < pauseMessageWidth; ++x) {
			std::cout << board.getChar({ pauseMessageX + x,pauseMessageY + y }); // Retrive current board characters
		}
	}
}

/**
 * @brief Displays the countdown before resuming the game.
 */
void GameFromInput::displayCountdown() {

	drawCharacters(); // Show characters while screen is paused
	gotoxy(pauseMessageX + countdownMessageTitleOffset, pauseMessageY + 1);
	std::cout << "CONTINUING IN:";
	gotoxy(pauseMessageX + countdownMessageTitleOffset, pauseMessageY + 2);
	std::cout << " ";

	for (int i = 3; i > 0; --i) {
		// Display the countdown number below the message
		gotoxy(pauseMessageX + countdownMessageCounterOffset, pauseMessageY + 2);
		std::cout << i << " ";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

/**
 * @brief Handles the actions when the player wins the game.
 * Resets the game and displays the "You Win" screen.
 */
void GameFromInput::handleGameWin() {
	currLevel = 0;
	lives = initLives;
	resetStage();
	screen.printWinScreen(score);
	Sleep(3500); // Pause to allow the player to see the screen
	gameState = GameState::MENU;
	eatBuffer();
	score = 0;
}

/**
 * @brief Handles the actions when the player loses the game.
 * Resets the game and displays the "Game Over" screen.
 */
void GameFromInput::handleGameOver() {
	currLevel = 0;
	lives = initLives;
	screen.printLoseScreen(score);
	Sleep(3500);
	gameState = GameState::MENU;
	eatBuffer();
}

/**
 * @brief Attempts to load the next valid board from the file list.
 * Skips invalid boards and returns false if no valid boards remain.
 */
bool GameFromInput::tryLoadNextValidBoard() {
	std::string errorMessage;

	while (currLevel < fileNames.size()) {
		board = Board();
		if (!board.load(fileNames[currLevel], &errorMessage)) {
			board.clearScreen();
			std::cout << "Board " << fileNames[currLevel] << " is not valid.\n\n" << errorMessage << std::endl;
			std::cout << "Press any key to try the next board...";
			_getch();
			eatBuffer();
			currLevel++;
		}
		else {
			if (save) {
				std::string filename_prefix = fileNames[currLevel].substr(0, fileNames[currLevel].find_last_of('.'));
				stepsFilename = filename_prefix + ".steps";
				resultsFilename = filename_prefix + ".result";

				steps.clearSteps();
				results.clearResults();
				long random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
				steps.setRandomSeed(random_seed);
				srand(random_seed);
			}
			
			currLevel++;
			iteration = 0;
			return true;
		}
	}
}
