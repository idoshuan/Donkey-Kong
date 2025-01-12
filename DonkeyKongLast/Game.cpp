#include "Game.h"

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
		startNewStage();
		gameState = GameState::PLAYING;
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
		if (currLevel == fileNames.size()) {
			gameState = GameState::WON;
		}
		else {
			startNewStage();
			gameState = GameState::PLAYING;
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
void Game::updateGameLogic() {
	checkHammerPickUp();
	drawCharacters();
	checkForKeyPress();
	checkSwing();
	eraseCharacters();
	trySpawnBarrel();
	explodeBarrels();
	checkGhostsCollision();
	if (checkMarioDeath()) {
		lives--;
		marioBlinkAnimation();
		resetStage();
		return;
	}
	if (checkMarioWon()) {
		marioBlinkAnimation();
		gameState = GameState::LEVEL_WON;
		return;
	}
	deactivateBarrels();
	moveCharacters();
}

/**
 * @brief Resets the game to prepare for a new round.
 * Clears barrels, resets Mario, and initializes game state variables.
 */
void Game::resetStage() {
	hammer.reset();
	board.reviveHammer();
	barrels.clear();
	for (size_t i = 0; i < ghosts.size(); ++i) {
		ghosts[i].setPos(board.getGhostsPos()[i]);
		ghosts[i].activate();
	}
	mario = Mario(board);
	firstBarrelSpawned = false;
	gameStartTime = clock::now();
	lastBarrelTime = gameStartTime;
	displayLives();
	eatBuffer();
}

void Game::startNewStage() {
	hammer.reset();
	gameStartTime = clock::now();
	board = Board();
	board.load(fileNames[currLevel++]);
	ghosts.clear();
	barrels.clear();
	mario = Mario(board);
	for (auto& ghostPos : board.getGhostsPos()) {
		ghosts.push_back(Ghost(ghostPos, board));
	}
	leftBarrelPos = { board.getDonkeyKongPos().getX() - 1, board.getDonkeyKongPos().getY() };
	rightBarrelPos = { board.getDonkeyKongPos().getX() + 1, board.getDonkeyKongPos().getY() };
	board.reset();
	board.print();
	displayLives();
}




// ------------------- Input Handling -------------------

/**
 * @brief Checks for user input during gameplay.
 * Processes ESC to pause the game or sends key input to Mario.
 */
void Game::checkForKeyPress() {
	for (int i = 0; i < 5; i++) {
		Sleep(12);
		if (_kbhit()) {
			KEYS key = charToKey(_getch());
			if (key != KEYS::INVALID) {
				if (key == KEYS::ESC) {
					gameState = GameState::PAUSED;
					return;
				}
				else if (key != KEYS::HAMMER) {
					mario.keyPressed(key);
				}
				else if (hammer && key == KEYS::HAMMER) {
					hammer->swing();
					hammerHitAnimation();
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
		menu.displayBoardFiles(fileNames);
		currLevel = menu.getScreenChoice(fileNames.size());
		gameState = GameState::START;
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
	Point paulina = board.getPaulinaPos();
	return mario.getX() + mario.getDirX() == paulina.getX() && mario.getY() == paulina.getY();
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

bool Game::checkMarioDeathFromGhost() {
	for (auto& ghost : ghosts) {
		if (ghost.isCurrentlyActive()) {
			if (isDirectCollision(ghost) || isMissedCollision(ghost)) {
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
	return hasBarrelExploded(barrel) || barrel.getX() == board.getMinX() || barrel.getX() == board.getMaxX();
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

void Game::checkGhostsCollision() {
	for (int i = 0; i < ghosts.size(); i++)
		for (int j = i + 1; j < ghosts.size(); j++) {
			if (ghosts[i].getY() == ghosts[j].getY())
				if (ghosts[i].getX() + ghosts[i].getDirX() == ghosts[j].getX() || ghosts[j].getX() + ghosts[j].getDirX() == ghosts[i].getX()) {
					ghosts[i].turnAround();
					ghosts[j].turnAround();
				}
		}
}




// ------------------- Hammer-Related Functions -------------------
void Game::checkHammerPickUp() {
	if (mario.getPos() == board.getHammerPos() && !hammer) {
		hammer.emplace(mario);
		board.deleteHammer();
	}
}

void Game::checkSwing() {
	if (hammer && hammer->isCurrentlySwinging()) {
		for (auto& barrel : barrels) {
			if (barrel.getPos() == hammer->getPos() || barrel.getNextPos() == hammer->getPos()) {
				barrel.deactivate();
			}
		}
		for (auto& ghost : ghosts) {
			if (ghost.getPos() == hammer->getPos() || ghost.getNextPos() == hammer->getPos()) {
				ghost.deactivate();
			}
		}
		hammer->stopSwing();
	}
}




// ------------------- Collision and Explosion Checks -------------------

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
 * @brief Checks for a direct collision between Mario and a barrel.
 */
bool Game::isDirectCollision(const Entity& entity) const {
	return mario.getX() == entity.getX() && mario.getY() == entity.getY();
}

/**
 * @brief Checks if Mario narrowly missed a collision with a barrel.
 */
bool Game::isMissedCollision(const Entity& entity) const {
	int marioPreviousX = mario.getX() - mario.getDirX();
	int marioPreviousY = mario.getY() - mario.getDirY();
	return marioPreviousX == entity.getX() && marioPreviousY == entity.getY();
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
	for (auto& ghost : ghosts) {
		if (ghost.isCurrentlyActive()) {
			ghost.move();
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
	for (auto& ghost : ghosts) {
		if (ghost.isCurrentlyActive()) {
			ghost.draw();
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
	for (auto& ghost : ghosts) {
		ghost.erase();
	}
}

/**
 * @brief Handles the actions when the player wins the game.
 * Resets the game and displays the "You Win" screen.
 */
void Game::handleGameWin() {
	currLevel = 0;
	lives = initLives;
	resetStage();
	screen.printWinScreen();
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
	lives = initLives;
	screen.printLoseScreen();
	Sleep(3500); // Pause to allow the player to see the screen
	gameState = GameState::MENU;
	eatBuffer();
}

/**
 * @brief Displays the current number of lives on the game screen.
 */
void Game::displayLives() const {
	int displayX = 6;
	int displayY = 0;

	gotoxy(displayX, displayY);
	std::cout << lives;
}




// ------------------- Animation Functions -------------------

/**
 * @brief Makes Mario blink (disappear and reappear) visually.
 */
void Game::marioBlinkAnimation() {
	drawCharacters();
	for (int i = 0; i < blinkIterations; i++) {
		mario.draw();
		Sleep(200);
		mario.erase();
		Sleep(200);
	}
	eraseCharacters();
}

void Game::hammerHitAnimation() {
	gotoxy(hammer->getX(), hammer->getY());
	std::cout << '+';
	Sleep(20);
}


