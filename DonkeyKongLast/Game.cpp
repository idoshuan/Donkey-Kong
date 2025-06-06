﻿#include "Game.h"

// ------------------- Game Loop -------------------

/**
 * @brief Starts the main game loop.
 * Continuously runs while the game is active (`isRunning`) and delegates control to `handleGameState()`.
 */
void Game::startGame() {
	ShowConsoleCursor(false);
	getBoardFileNames(fileNames);
	while (isRunning) {
		handleGameState();
	}
}

/**
 * @brief Handles the logic for the current game state.
 * Switches between states like MENU, START, PLAYING, PAUSED, GAME_OVER, and WON.
 */
void Game::handleGameState() {
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
		if (lives > 2) {
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
		steps.saveSteps(stepsFilename);
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
void Game::updateGameLogic() {
	iteration++;
	checkHammerPickUp();
	drawCharacters();
	checkForKeyPress();
	checkKill();
	eraseCharacters();
	trySpawnBarrel();
	explodeBarrels();
	checkGhostWithGhostCollisions();
	deactivateBarrels();
	moveCharacters();
	if (checkMarioDeath()) {
		lives--;
		marioBlinkAnimation();
		resetStage();
		return;
	}
	if (checkMarioWon()) {
		score += stageFinishPoints;
		scoreAnimation(stageFinishPointsString);
		marioBlinkAnimation();
		gameState = GameState::LEVEL_WON;
		return;
	}
}





// ------------------- Input Handling -------------------

/**
 * @brief Checks for user input during gameplay.
 * Processes ESC to pause the game or sends key input to Mario.
 */
void Game::checkForKeyPress() {
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
					steps.addStep({ iteration,ENTITIES_CHARACTERS::MARIO,key });
					mario.keyPressed(key);
				}
				else if (hammer && key == KEYS::HAMMER && !hammerKeyPressed) {
					hammerKeyPressed = true;
					steps.addStep({ iteration,ENTITIES_CHARACTERS::HAMMER,key });
					hammer->swing();
				}
			}
		}
	}
}

/**
 * @brief Handles menu actions like starting the game, showing instructions, or exiting.
 */
void Game::handleMenuState(MenuAction action) {
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




// ------------------- Mario-Related Functions -------------------

/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool Game::checkMarioDeath() {
	return checkMarioDeathFromBarrel() || checkMarioDeathFromFall() || checkMarioDeathFromGhost();
}

/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool Game::checkMarioWon() {
	return mario.getNextPos() == board.getPaulinaPos() || mario.getPos() == board.getPaulinaPos();
}

/**
 * @brief Checks if Mario has died from a barrel collision.
 */
bool Game::checkMarioDeathFromBarrel() {
	for (auto& barrel : barrels) {
		if (barrel.isCurrentlyActive()) {
			if (isDirectCollision(barrel) || isMissedCollision(barrel) || isExplosionFatal(barrel)) {
				return true;
			}
		}
	}
	return false;
}

/**
 * @brief Checks if Mario has fallen beyond the maximum allowed fall height.
 */
bool Game::checkMarioDeathFromFall() {
	if (mario.getFallingCounter() >= marioMaxFallHeight && !mario.isCurrentlyFalling()) {
		return true;
	}
	else {
		mario.resetFallingCounterIfNeeded();
		return false;
	}
}

/**
 * @brief Checks if Mario has died from a ghost collision.
 */
bool Game::checkMarioDeathFromGhost() {

	for (auto& ghost : ghostContainer) {
		if (ghost->isCurrentlyActive()) {
			if (isDirectCollision(*ghost) || isMissedCollision(*ghost)) {
				return true;
			}
		}
	}
	return false;
}




// ------------------- Barrel-Related Functions -------------------

/**
 * @brief Attempts to spawn barrels based on elapsed game time.
 * Note: The time management logic in these functions was created with the help of ChatGPT.
 */
void Game::trySpawnBarrel() {
	auto now = clock::now();

	if (shouldSpawnFirstBarrel(now)) {
		firstBarrelSpawned = true;
	}

	if (canSpawnBarrel(now)) {
		spawnBarrel();
		lastBarrelTime = now;
	}
}

/**
 * @brief Determines if the first barrel should be spawned.
 * Compares the elapsed game time to the initial spawn delay.
 */
bool Game::shouldSpawnFirstBarrel(const time& now) const {
	if (firstBarrelSpawned) return false;
	auto gameElapsedTime = std::chrono::duration_cast<milliseconds>(now - gameStartTime).count();
	return gameElapsedTime > barrelInitialSpawnDelay;
}

/**
 * @brief Checks if additional barrels can be spawned.
 * Ensures conditions like timing and barrel count are met.
 */
bool Game::canSpawnBarrel(const time& now) const {
	if (!firstBarrelSpawned) return false;
	auto timeSinceLastBarrelSpawn = std::chrono::duration_cast<milliseconds>(now - lastBarrelTime).count();
	return timeSinceLastBarrelSpawn >= barrelSpawnInterval;
}

/**
 * @brief Spawns a new barrel at the left or right position.
 * Activates the newly created barrel and increments the barrel count.
 */
void Game::spawnBarrel() {
	if (barrels.size() % 2 == 0) {
		barrels.push_back(Barrel(leftBarrelPos, board));
	}
	else {
		barrels.push_back(Barrel(rightBarrelPos, board));
	}
	barrels.back().activate();
}

/**
 * @brief Checks if a barrel should be deactivated.
 * Considers explosions and position boundaries.
 */
bool Game::shouldDeactivateBarrel(Barrel& barrel) const {
	return hasBarrelExploded(barrel) || barrel.getX() + barrel.getDirX() == board.getMinX() || barrel.getX() + barrel.getDirX() == board.getMaxX();
}

/**
 * @brief Handles explosions of barrels on the game board.
 */
void Game::explodeBarrels() {
	for (auto& barrel : barrels) {
		if (hasBarrelExploded(barrel)) {
			barrel.explode();
		}
	}
}

/**
 * @brief Deactivates barrels based on certain conditions.
 */
void Game::deactivateBarrels() {
	for (auto& barrel : barrels) {
		if (shouldDeactivateBarrel(barrel)) {
			barrel.deactivate();
		}
	}
}

/**
 * @brief Checks if a barrel has exploded.
 */
bool Game::hasBarrelExploded(Barrel& barrel) const {
	if (barrel.getFallingCounter() >= barrelMaxFallHeight && !barrel.isCurrentlyFalling()) {
		return true;
	}
	else {
		barrel.resetFallingCounterIfNeeded();
		return false;
	}
}




// ------------------- Ghost-Related Functions -------------------

/**
 * @brief Handles collisions between ghosts on the same Y-coordinate.
 * Activates collision logic if their paths intersect.
 */
void Game::checkGhostWithGhostCollisions() {
	for (int i = 0; i < ghostContainer.size(); i++) {
		for (int j = i + 1; j < ghostContainer.size(); j++) {

			bool isAtSamePositionOrNext = (ghostContainer[i]->getNextPos() == ghostContainer[j]->getPos()) ||
				(ghostContainer[i]->getNextPos() == ghostContainer[j]->getNextPos());

			bool areDirectionsDifferent = (ghostContainer[i]->getDirX() != ghostContainer[j]->getDirX()) &&
				(ghostContainer[i]->getDirY() == -ghostContainer[j]->getDirY());

			if (isAtSamePositionOrNext && areDirectionsDifferent) {
				ghostContainer[i]->collision();
				ghostContainer[j]->collision();
			}
		}
	}
}




// ------------------- Hammer-Related Functions -------------------
/**
 * @brief Checks if Mario picks up the hammer.
 * Activates the hammer if Mario reaches its position.
 */
void Game::checkHammerPickUp() {
	if (mario.getPos() == board.getHammerPos() && !hammer) {
		hammer.emplace(mario);
		board.deleteHammer();
	}
}

/**
 * @brief Handles entity kills while the hammer is swinging.
 * Deactivates barrels and ghosts hit by the hammer, awarding points.
 */
void Game::checkKill() {
	if (hammer && hammer->isCurrentlySwinging()) {
		for (auto& barrel : barrels) {
			if (barrel.isCurrentlyActive() && (barrel.getPos() == hammer->getPos() || barrel.getNextPos() == hammer->getPos())) {
				barrel.deactivate();
				score += barrelKillPoints;
				scoreAnimation(barrelKillPointsString);
			}
		}
		for (auto& ghost : ghostContainer) {
			if (ghost->isCurrentlyActive() && (ghost->getPos() == hammer->getPos() || ghost->getNextPos() == hammer->getPos())) {
				ghost->deactivate();
				score += ghostKillPoints;
				scoreAnimation(ghostKillPointsString);
			}
		}
		if (hammer && hammer->isCurrentlySwinging()) {
			hammerHitAnimation();
		}
		hammer->stopSwing();
	}
}




// ------------------- Collision and Explosion Checks -------------------

/**
 * @brief Checks for a direct collision between Mario and a enemy entity.
 */
bool Game::isDirectCollision(const Entity& entity) const {
	return mario.getX() == entity.getX() && mario.getY() == entity.getY();
}

/**
 * @brief Checks if Mario narrowly missed a collision with a enemy entity.
 */
bool Game::isMissedCollision(const Entity& entity) const {
	int marioPreviousX = mario.getX() - mario.getDirX();
	int marioPreviousY = mario.getY() - mario.getDirY();
	return marioPreviousX == entity.getX() && marioPreviousY == entity.getY();
}

/**
 * @brief Checks if Mario is within the explosion radius of a barrel.
 */
bool Game::isInExplosionRadius(const Barrel& barrel) const {
	int barrelX = barrel.getX();
	int barrelY = barrel.getY();

	int marioX = mario.getX();
	int marioY = mario.getY();

	int difX = std::abs(barrelX - marioX);
	int difY = barrelY - marioY;

	return (difX <= barrelExplosionRadius && difY >= 0 && difY <= barrelExplosionRadius);
}


/**
 * @brief Checks if a barrel explosion is fatal to Mario.
 */
bool Game::isExplosionFatal(const Barrel& barrel) const {
	return barrel.didExplode() && isInExplosionRadius(barrel);
}




// ------------------- Pause Handling -------------------

/**
 * @brief Handles the pause state during gameplay.
 * Displays the pause screen and waits for ESC to resume.
 */
void Game::handlePause() {
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
void Game::displayPauseScreen() {
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
void Game::clearMessageInsideBorders() {
	for (int y = 1; y < pauseMessageHeight - 2; ++y) {
		gotoxy(pauseMessageX + pauseMessageContinueOffset, pauseMessageY + y);
		for (int x = 0; x < pauseMessageWidth - 2; ++x) {
			std::cout << ' '; // Clear inner content character by character
		}
	}
}

/**
 * @brief Displays the countdown before resuming the game.
 */
void Game::displayCountdown() {

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
 * @brief Clears the pause screen.
 */
void Game::clearEntirePauseScreen() {
	for (int y = 0; y < pauseMessageHeight; ++y) {
		gotoxy(pauseMessageX, pauseMessageY + y);
		for (int x = 0; x < pauseMessageWidth; ++x) {
			std::cout << board.getChar({ pauseMessageX + x,pauseMessageY + y }); // Retrive current board characters
		}
	}
}




// ------------------- Utility Functions -------------------

/**
 * @brief Moves Mario and barrels on the game board.
 */
void Game::moveCharacters() {
	mario.move();
	if (hammer) {
		hammer->move();
	}
	for (auto& barrel : barrels) {
		if (barrel.isCurrentlyActive()) {
			barrel.move();
		}
	}
	for (auto& ghost : ghostContainer) {
		if (ghost->isCurrentlyActive()) {
			ghost->move();
		}
	}
}

/**
 * @brief Draws Mario and barrels on the game board.
 */
void Game::drawCharacters() {
	mario.draw();
	if (hammer) {
		hammer->draw();
	}
	for (auto& barrel : barrels) {
		if (barrel.isCurrentlyActive()) {
			barrel.draw();
		}
	}
	for (auto& ghost : ghostContainer) {
		if (ghost->isCurrentlyActive()) {
			ghost->draw();
		}
	}
}

/**
 * @brief Erases Mario and barrels from the game board.
 */
void Game::eraseCharacters() {
	mario.erase();
	if (hammer) {
		hammer->erase();
	}
	for (auto& barrel : barrels) {
		barrel.erase();
	}
	for (auto& ghost : ghostContainer) {
		ghost->erase();
	}
}

/**
 * @brief Displays the current number of lives on the game screen.
 */
void Game::displayLives() const {
	int displayX = board.getLegendPos().getX();
	int displayY = board.getLegendPos().getY();

	gotoxy(displayX, displayY);
	std::cout << "LIVES: " << lives;
}

void Game::displayScore() const {
	int displayX = board.getLegendPos().getX();
	int displayY = board.getLegendPos().getY() + 1;

	gotoxy(displayX, displayY);
	std::cout << "SCORE: " << score;
}




// ------------------- Stage Functions -------------------

/**
 * @brief Resets the stage for a new round.
 * Reinitializes barrels, Mario, ghosts, and game state variables.
 */
void Game::resetStage() {
	if (hammer) {
		hammer.reset();
		board.reviveHammer();
	}
	ghostContainer.clear();
	barrels.clear();
	for (const auto& ghostPos : board.getGhostsPos()) {
		ghostContainer.push_back(std::make_unique<Ghost>(ghostPos, board));
	}
	for (const auto& ghostPos : board.getClimbingGhostsPos()) {
		ghostContainer.push_back(std::make_unique<ClimbingGhost>(ghostPos, board));
	}
	mario = Mario(board);
	firstBarrelSpawned = false;
	gameStartTime = clock::now();
	lastBarrelTime = gameStartTime;
	displayLives();
	eatBuffer();
}

/**
 * @brief Starts a new stage by loading the next board.
 * Initializes Mario, ghosts, barrels, and updates the game display.
 */
void Game::startNewStage() {

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

/**
 * @brief Handles the actions when the player wins the game.
 * Resets the game and displays the "You Win" screen.
 */
void Game::handleGameWin() {
	currLevel = 0;
	lives = initLives;
	resetStage();
	screen.printWinScreen(score);
	Sleep(3500); // Pause to allow the player to see the screen
	gameState = GameState::MENU;
	eatBuffer();
}

/**
 * @brief Handles the actions when the player loses the game.
 * Resets the game and displays the "Game Over" screen.
 */
void Game::handleGameOver() {
	currLevel = 0;
	score = 0;
	lives = initLives;
	screen.printLoseScreen(score);
	Sleep(3500);
	gameState = GameState::MENU;
	eatBuffer();
}





// ------------------- File Management Functions -------------------

/**
 * @brief Retrieves all valid game board files from the current directory.
 * Filters files starting with "dkong_" and ending with ".screen", then sorts them lexicographically.
 */
void Game::getBoardFileNames(std::vector<std::string>& fileNames) {
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
 * @brief Attempts to load the next valid board from the file list.
 * Skips invalid boards and returns false if no valid boards remain.
 */
bool Game::tryLoadNextValidBoard() {
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
			std::string filename_prefix = fileNames[currLevel].substr(0, fileNames[currLevel].find_last_of('.'));
			stepsFilename = filename_prefix + ".steps";

			long random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
			steps.clearSteps();
			steps.setRandomSeed(random_seed);
			srand(random_seed);

			iteration = 0;
			currLevel++;
			return true;
		}
	}

	board.clearScreen();
	return false;
}



// ------------------- Animation Functions -------------------

/**
 * @brief Makes Mario blink (disappear and reappear) visually.
 */
void Game::marioBlinkAnimation() {
	for (int i = 0; i < blinkIterations; i++) {
		mario.draw();
		Sleep(200);
		mario.erase();
		Sleep(200);
	}
	eraseCharacters();
}

/**
 * @brief Displays a visual effect for a hammer hit.
 */
void Game::hammerHitAnimation() {
	if (board.isValidPosition({ hammer->getX(), hammer->getY() - 1 })) {
		gotoxy(hammer->getX(), hammer->getY());
		std::cout << static_cast<char>(BOARD_CHARACTERS::AIR);
		gotoxy(hammer->getX(), hammer->getY() - 1);
		std::cout << static_cast<char>(BOARD_CHARACTERS::HAMMER);
		Sleep(18);
		gotoxy(hammer->getX(), hammer->getY() - 1);
		std::cout << board.getChar({ hammer->getX(), hammer->getY() - 1 });
	}
}

/**
 * @brief Animates the display of score points above Mario's position.
 * Checks valid positions and briefly shows the points before restoring the board.
 */
void Game::scoreAnimation(const std::string& points) {
	if (board.isValidPosition({ mario.getX() - 1, mario.getY() - 1 }) &&
		board.isValidPosition({ mario.getX(), mario.getY() - 1 }) &&
		board.isValidPosition({ mario.getX() + 1, mario.getY() - 1 })) {

		gotoxy(mario.getX() - 1, mario.getY() - 1);
		std::cout << points;
		Sleep(60);

		for (int x = 0; x < points.size(); ++x) {
			gotoxy(mario.getX() - 1 + x, mario.getY() - 1);
			std::cout << board.getChar({ mario.getX() - 1 + x, mario.getY() - 1 });
		}
	}
	displayScore();
}

