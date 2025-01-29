#include "GameFromFile.h"


// ------------------- Game Loop -------------------

/**
 * @brief Handles the logic for the current game state.
 * Switches between states like MENU, START, PLAYING, PAUSED, GAME_OVER, and WON.
 */
void GameFromFile::handleGameState() {
	switch (gameState) {
	case GameState::START:
		gameState = GameState::PLAYING;
		startNewStage();
		break;
	case GameState::PLAYING:
		if (lives > 0) {
			try {
				checkResultError();
				updateGameLogic();
			}
			catch (const std::runtime_error& e) {
				clearScreen();
				std::cout << e.what() << std::endl;
				isRunning = false;
			}
		}
		else {
			gameState = GameState::GAME_OVER;
		}
		break;
	case GameState::LEVEL_WON:
		if (currLevel == fileNames.size()) {
			gameState = GameState::WON;
		}
		else {
			gameState = GameState::PLAYING;
			startNewStage();
		}
		break;
	case GameState::GAME_OVER:
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


/**
*@brief Processes input from the steps file and updates Mario's or Hammer's state accordingly.
* Includes timing - based pauses for silent and loaded modes.
*/
void GameFromFile::checkForKeyPress() {
	while (steps.isNextStepOnIteration(iteration)) {
		KEYS key = steps.popStep(iteration);
		if (key == KEYS::HAMMER) {
			hammer->swing();
		}
		else {
			mario.keyPressed(key);
		}
	}
	Sleep(silent ? sleepForSilent : sleepForLoad);
}



/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool GameFromFile::checkMarioDeath() {
	if (checkMarioDeathFromBarrel() || checkMarioDeathFromFall() || checkMarioDeathFromGhost()) {
		int nextDeathIteration = results.getNextDeathIteration();

		if (nextDeathIteration == Results::NO_DEATH) {
			throw std::runtime_error(
				"Error on screen " + fileNames[currLevel-1] + ":\nMario died unexpectedly. No death was expected at iteration " +
				std::to_string(iteration) + "."
			);
		}

		if (nextDeathIteration != iteration) {
			throw std::runtime_error(
				"Error on screen " + fileNames[currLevel-1] + ":\nMario's death occurred at the wrong iteration. "
				"Expected iteration: " + std::to_string(nextDeathIteration) +
				", but got: " + std::to_string(iteration) + "."
			);
		}

		results.popResult();
		return true;
	}
	else if (results.getNextDeathIteration() == iteration) {
		throw std::runtime_error(
			"Error on screen " + fileNames[currLevel-1] + ":\nMario was expected to die in this iteration (" +
			std::to_string(iteration) + ") but did not."
		);
	}
	return false;
}


/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool GameFromFile::checkMarioWon() {
	if (mario.getNextPos() == board.getPaulinaPos() || mario.getPos() == board.getPaulinaPos()) {
		int finishedIteration = results.getFinishedIteration();

		if (finishedIteration == Results::NO_FINISH) {
			throw std::runtime_error(
				"Error on screen " + fileNames[currLevel-1] + ":\nMario finished unexpectedly.No finish was expected at iteration " +
				std::to_string(iteration) + "."
			);
		}

		if (finishedIteration != iteration) {
			throw std::runtime_error(
				"Error on screen " + fileNames[currLevel-1] + ":\nMario's finish stage occurred at the wrong iteration. "
				"Expected iteration: " + std::to_string(finishedIteration) +
				", but got: " + std::to_string(iteration) + "."
			);
		}

		results.popResult();
		return true;
	}
	else if (results.getFinishedIteration() == iteration) {
		throw std::runtime_error(
			"Error on screen " + fileNames[currLevel-1] + ":\nMario was expected to finish the stage in this iteration (" +
			std::to_string(iteration) + ") but did not."
		);
	}

	return false;
}

// ------------------- Stage Functions -------------------

/**
 * @brief Starts a new stage by loading the next board.
 * Resets Mario, barrels, and ghost positions, and initializes all necessary objects.
 * Updates the display in non-silent mode.
 */
void GameFromFile::startNewStage() {
	LoadNextBoard();

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
	if (!silent)board.print();
	displayScore();
	displayLives();
}



// ------------------- File Management Functions -------------------

/**
 * @brief Retrieves and sorts all valid game board files from the current directory.
 * Ensures that each `.screen` file has corresponding `.steps` and `.result` files.
 */
void GameFromFile::getBoardFileNames() {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 6) == "dkong_" && filename.extension() == ".screen") {
			std::string stepsFile = filenameStr.substr(0, filenameStr.find_last_of('.')) + ".steps";
			std::string resultsFile = filenameStr.substr(0, filenameStr.find_last_of('.')) + ".result";

			if (fs::exists(stepsFile) && fs::exists(resultsFile)) {
				fileNames.push_back(filenameStr); // Add the .screen file if .steps exists
				stepsFileNames.push_back(stepsFile);
				resultsFileNames.push_back(resultsFile);
			}

		}
	}
	if (fileNames.size() == 0) {
		throw std::runtime_error("No files to load from. Please play a game on save mode...");
	}

	std::sort(fileNames.begin(), fileNames.end());
	std::sort(stepsFileNames.begin(), stepsFileNames.end());
	std::sort(resultsFileNames.begin(), resultsFileNames.end());
}

/**
 * @brief Checks if results file hasn't ended prematurely
 */
void GameFromFile::checkResultError() {
	if (results.isFinishedBy(iteration)) {
		throw std::runtime_error(
			"Error on screen " + fileNames[currLevel - 1] + ":\nResults file finished but steps hasn't"
		);
	}
}

/**
 * @brief Handles the GAME_OVER state by clearing the screen and showing a "Test pass. You lose" message.
 * Ends the game loop.
 */
void GameFromFile::handleGameOver() {
	clearScreen();
	std::cout << "Mario lost but at least the test passed :)" << std::endl;
	isRunning = false;
}

/**
 * @brief Handles the WON state by clearing the screen and showing a "Test pass. You Won" message.
 * Ends the game loop.
 */
void GameFromFile::handleGameWin() {
	clearScreen();
	std::cout << "Test passed, mario survived through all screens." << std::endl;
	isRunning = false;
}



/**
 * @brief Attempts to load the next valid board from the list of files.
 * Skips invalid boards and sets up the game for the next level.
 * Initializes random seed based on the steps file.
 */
void GameFromFile::LoadNextBoard() {
	std::string stam;

	if (currLevel < fileNames.size()) {
		board = Board();
		std::string filename_prefix = fileNames[currLevel].substr(0, fileNames[currLevel].find_last_of('.'));
		board.load(fileNames[currLevel], &stam);
		steps = Steps::loadSteps(stepsFileNames[currLevel]);
		results = Results::loadResults(resultsFileNames[currLevel]);
		long random_seed = steps.getRandomSeed();
		srand(random_seed);

		currLevel++;
		iteration = 0;
	}
}
