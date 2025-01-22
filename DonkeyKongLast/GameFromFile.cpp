#include "GameFromFile.h"


// ------------------- Game Loop -------------------

/**
 * @brief Starts the main game loop.
 * Continuously runs while the game is active (`isRunning`) and delegates control to `handleGameState()`.
 */
void GameFromFile::startGame() {
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
void GameFromFile::handleGameState() {
	switch (gameState) {
	case FileGameState::START:
		gameState =FileGameState::PLAYING;
		startNewStage();
		break;
	case FileGameState::PLAYING:
		if (lives > 2) {
			updateGameLogic();
		}
		else {
			gameState = FileGameState::GAME_OVER;
		}
		break;
	case FileGameState::LEVEL_WON:
		if (currLevel == fileNames.size()) {
			gameState = FileGameState::WON;
		}
		else {
			gameState = FileGameState::PLAYING;
			startNewStage();
		}
		break;
	case FileGameState::GAME_OVER:
		handleGameOver();
		break;
	case FileGameState::WON:
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
	drawCharacters();
	checkForKeyPress(iteration);
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
		gameState = FileGameState::LEVEL_WON;
		return;
	}
}

void GameFromFile::checkForKeyPress(size_t iteration) {
	if (steps.isNextStepOnIteration(iteration)) {
		Step step = steps.popStep(iteration);
		if (step.entity == ENTITIES_CHARACTERS::MARIO) {
			mario.keyPressed(step.key);
		}
		else if (step.entity == ENTITIES_CHARACTERS::HAMMER) {
			hammer->swing();
		}
	}
	Sleep(70);
}




// ------------------- Mario-Related Functions -------------------

/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool GameFromFile::checkMarioDeath() {
	return checkMarioDeathFromBarrel() || checkMarioDeathFromFall() || checkMarioDeathFromGhost();
}

/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool GameFromFile::checkMarioWon() {
	return mario.getNextPos() == board.getPaulinaPos() || mario.getPos() == board.getPaulinaPos();
}

/**
 * @brief Checks if Mario has died from a barrel collision.
 */
bool GameFromFile::checkMarioDeathFromBarrel() {
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
bool GameFromFile::checkMarioDeathFromFall() {
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
bool GameFromFile::checkMarioDeathFromGhost() {

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
void GameFromFile::trySpawnBarrel() {
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
bool GameFromFile::shouldSpawnFirstBarrel(const time& now) const {
	if (firstBarrelSpawned) return false;
	auto gameElapsedTime = std::chrono::duration_cast<milliseconds>(now - gameStartTime).count();
	return gameElapsedTime > barrelInitialSpawnDelay;
}

/**
 * @brief Checks if additional barrels can be spawned.
 * Ensures conditions like timing and barrel count are met.
 */
bool GameFromFile::canSpawnBarrel(const time& now) const {
	if (!firstBarrelSpawned) return false;
	auto timeSinceLastBarrelSpawn = std::chrono::duration_cast<milliseconds>(now - lastBarrelTime).count();
	return timeSinceLastBarrelSpawn >= barrelSpawnInterval;
}

/**
 * @brief Spawns a new barrel at the left or right position.
 * Activates the newly created barrel and increments the barrel count.
 */
void GameFromFile::spawnBarrel() {
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
bool GameFromFile::shouldDeactivateBarrel(Barrel& barrel) const {
	return hasBarrelExploded(barrel) || barrel.getX() + barrel.getDirX() == board.getMinX() || barrel.getX() + barrel.getDirX() == board.getMaxX();
}

/**
 * @brief Handles explosions of barrels on the game board.
 */
void GameFromFile::explodeBarrels() {
	for (auto& barrel : barrels) {
		if (hasBarrelExploded(barrel)) {
			barrel.explode();
		}
	}
}

/**
 * @brief Deactivates barrels based on certain conditions.
 */
void GameFromFile::deactivateBarrels() {
	for (auto& barrel : barrels) {
		if (shouldDeactivateBarrel(barrel)) {
			barrel.deactivate();
		}
	}
}

/**
 * @brief Checks if a barrel has exploded.
 */
bool GameFromFile::hasBarrelExploded(Barrel& barrel) const {
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
void GameFromFile::checkGhostWithGhostCollisions() {
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
void GameFromFile::checkHammerPickUp() {
	if (mario.getPos() == board.getHammerPos() && !hammer) {
		hammer.emplace(mario);
		board.deleteHammer();
	}
}

/**
 * @brief Handles entity kills while the hammer is swinging.
 * Deactivates barrels and ghosts hit by the hammer, awarding points.
 */
void GameFromFile::checkKill() {
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
bool GameFromFile::isDirectCollision(const Entity& entity) const {
	return mario.getX() == entity.getX() && mario.getY() == entity.getY();
}

/**
 * @brief Checks if Mario narrowly missed a collision with a enemy entity.
 */
bool GameFromFile::isMissedCollision(const Entity& entity) const {
	int marioPreviousX = mario.getX() - mario.getDirX();
	int marioPreviousY = mario.getY() - mario.getDirY();
	return marioPreviousX == entity.getX() && marioPreviousY == entity.getY();
}

/**
 * @brief Checks if Mario is within the explosion radius of a barrel.
 */
bool GameFromFile::isInExplosionRadius(const Barrel& barrel) const {
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
bool GameFromFile::isExplosionFatal(const Barrel& barrel) const {
	return barrel.didExplode() && isInExplosionRadius(barrel);
}





// ------------------- Utility Functions -------------------

/**
 * @brief Moves Mario and barrels on the game board.
 */
void GameFromFile::moveCharacters() {
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
void GameFromFile::drawCharacters() {
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
void GameFromFile::eraseCharacters() {
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
void GameFromFile::displayLives() const {
	int displayX = board.getLegendPos().getX();
	int displayY = board.getLegendPos().getY();

	gotoxy(displayX, displayY);
	std::cout << "LIVES: " << lives;
}

void GameFromFile::displayScore() const {
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
void GameFromFile::resetStage() {
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

/**
 * @brief Handles the actions when the player wins the game.
 * Resets the game and displays the "You Win" screen.
 */
void GameFromFile::handleGameWin() {
	currLevel = 0;
	lives = initLives;
	resetStage();
	screen.printWinScreen(score);
	Sleep(3500); // Pause to allow the player to see the screen
	eatBuffer();
}

/**
 * @brief Handles the actions when the player loses the game.
 * Resets the game and displays the "Game Over" screen.
 */
void GameFromFile::handleGameOver() {
	currLevel = 0;
	score = 0;
	lives = initLives;
	screen.printLoseScreen(score);
	Sleep(3500);
	eatBuffer();
}





// ------------------- File Management Functions -------------------

/**
 * @brief Retrieves all valid game board files from the current directory.
 * Filters files starting with "dkong_" and ending with ".screen", then sorts them lexicographically.
 */
void GameFromFile::getBoardFileNames(std::vector<std::string>& fileNames) {
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



// ------------------- Animation Functions -------------------

/**
 * @brief Makes Mario blink (disappear and reappear) visually.
 */
void GameFromFile::marioBlinkAnimation() {
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
void GameFromFile::hammerHitAnimation() {
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
void GameFromFile::scoreAnimation(const std::string& points) {
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

