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
			updateGameLogic();
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


void GameFromFile::checkForKeyPress() {
	if (steps.isNextStepOnIteration(iteration)) {
		Step step = steps.popStep(iteration);
		if (step.entity == ENTITIES_CHARACTERS::MARIO) {
			mario.keyPressed(step.key);
		}
		else if (step.entity == ENTITIES_CHARACTERS::HAMMER) {
			hammer->swing();
		}
	}
	Sleep(30);
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
	gameStartTime = clock::now();
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

			if (fs::exists(stepsFile)) {
				fileNames.push_back(filenameStr); // Add the .screen file if .steps exists
				stepsFileNames.push_back(stepsFile);
			}
		}
	}
	std::sort(fileNames.begin(), fileNames.end());
	std::sort(stepsFileNames.begin(), stepsFileNames.end());
}

/**
 * @brief Attempts to load the next valid board from the file list.
 * Skips invalid boards and returns false if no valid boards remain.
 */
void GameFromFile::LoadNextBoard() {
	std::string stam;

	if (currLevel < fileNames.size()) {
		std::string filename_prefix = fileNames[currLevel].substr(0, fileNames[currLevel].find_last_of('.'));
		board.load(fileNames[currLevel], &stam);
		steps = Steps::loadSteps(stepsFileNames[currLevel]);
		long random_seed = steps.getRandomSeed();
		currLevel++;
		iteration = 0;
	}
}