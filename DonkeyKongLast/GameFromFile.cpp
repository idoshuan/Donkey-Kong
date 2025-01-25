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
		if (lives > 2) {
			try {
				updateGameLogic();
			}
			catch (const std::string& e) {
				clearScreen();
				std::cout << e << std::endl;
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
 * @brief Updates the game logic during the PLAYING state.
 * Handles input, updates Mario and barrels, and checks for game-ending conditions.
 */
void GameFromFile::updateGameLogic() {
	iteration++;
	checkHammerPickUp();
	if (!isSilent)drawCharacters();
	checkForKeyPress();
	checkKill();
	if (!isSilent)eraseCharacters();
	trySpawnBarrel();
	explodeBarrels();
	checkGhostWithGhostCollisions();
	deactivateBarrels();
	moveCharacters();
	if (checkMarioDeath()) {
		lives--;
		if (!isSilent)marioBlinkAnimation();
		resetStage();
		return;
	}
	if (checkMarioWon()) {
		score += stageFinishPoints;
		if (!isSilent) {
			scoreAnimation(stageFinishPointsString);
			marioBlinkAnimation();
		}
		gameState = GameState::LEVEL_WON;
		return;
	}
}

void GameFromFile::checkForKeyPress() {
	if (steps.isNextStepOnIteration(iteration)) {
		KEYS key = steps.popStep(iteration);
		if (key == KEYS::HAMMER) {
			hammer->swing();
		}
		else {
			mario.keyPressed(key);
		}
	}
	Sleep(isSilent ? 0 : 30);
}



/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool GameFromFile::checkMarioDeath() {
	if (checkMarioDeathFromBarrel() || checkMarioDeathFromFall() || checkMarioDeathFromGhost()) {
		if (results.getNextDeathIteration() != iteration) {
			std::ostringstream error;
			error << "Error: Mario's death occurred at the wrong iteration. "
				<< "Expected iteration: " << results.getNextDeathIteration()
				<< ", but got: " << iteration << ".";
			throw error.str();
		}
		results.popResult();
		return true;
	}
	else if (results.getNextDeathIteration() == iteration) {
		std::ostringstream error;
		error << "Error: Mario was expected to die in this iteration ("
			<< iteration << ") but did not.";
		throw error.str();
	}
	return false;
}


/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool GameFromFile::checkMarioWon() {
	if (mario.getNextPos() == board.getPaulinaPos() || mario.getPos() == board.getPaulinaPos()) {
		if (results.getFinishedIteration() != iteration) {
			std::ostringstream error;
			error << "Error: Mario's finish stage occurred at the wrong iteration. "
				<< "Expected iteration: " << results.getFinishedIteration()
				<< ", but got: " << iteration << ".";
			throw error.str();
		}
		results.popResult();
		return true;
	}
	else if (results.getFinishedIteration() == iteration) {
		std::ostringstream error;
		error << "Error: Mario was expected to finish the stage in this iteration ("
			<< iteration << ") but did not.";
		throw error.str();
	}
	return false;
}

// ------------------- Stage Functions -------------------

/**
 * @brief Starts a new stage by loading the next board.
 * Initializes Mario, ghosts, barrels, and updates the game display.
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
	if (!isSilent) {
		board.print();
		displayScore();
		displayLives();
	}
}



// ------------------- File Management Functions -------------------

/**
 * @brief Retrieves all valid game board files from the current directory.
 * Filters files starting with "dkong_" and ending with ".screen", then sorts them lexicographically.
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
	std::sort(fileNames.begin(), fileNames.end());
	std::sort(stepsFileNames.begin(), stepsFileNames.end());
	std::sort(resultsFileNames.begin(), resultsFileNames.end());
}

void GameFromFile::handleGameOver() {
	clearScreen();
	std::cout << "Test pass. You lose" << std::endl;
	isRunning = false;
}

void GameFromFile::handleGameWin() {
	clearScreen();
	std::cout << "Test pass. You Won" << std::endl;
	isRunning = false;
}


/**
 * @brief Attempts to load the next valid board from the file list.
 * Skips invalid boards and returns false if no valid boards remain.
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

/**
 * @brief Displays the current number of lives on the game screen.
 */
void GameFromFile::displayLives() const{
	if (!isSilent) {
		int displayX = board.getLegendPos().getX();
		int displayY = board.getLegendPos().getY();
		gotoxy(displayX, displayY);
		std::cout << "LIVES: " << lives;
	}
}


