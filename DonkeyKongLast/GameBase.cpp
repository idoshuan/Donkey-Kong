#include "GameBase.h"

// ------------------- Game Loop -------------------

/**
 * @brief Starts the main game loop.
 * Continuously runs while the game is active (`isRunning`) and delegates control to `handleGameState()`.
 */
void GameBase::run() {
	ShowConsoleCursor(false);
	getBoardFileNames();
	while (isRunning) {
		handleGameState();
	}
}

/**
 * @brief Updates the game logic during the PLAYING state.
 * Handles input, updates Mario and barrels, and checks for game-ending conditions.
 */
void GameBase::updateGameLogic() {
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




// ------------------- Mario-Related Functions -------------------

/**
 * @brief Checks if Mario has died from a barrel collision.
 */
bool GameBase::checkMarioDeathFromBarrel() {
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
bool GameBase::checkMarioDeathFromFall() {
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
bool GameBase::checkMarioDeathFromGhost() {

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
void GameBase::trySpawnBarrel() {
	if (iteration % 40 == 0) {
		spawnBarrel();
	}
}

/**
 * @brief Spawns a new barrel at the left or right position.
 * Activates the newly created barrel and increments the barrel count.
 */
void GameBase::spawnBarrel() {
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
bool GameBase::shouldDeactivateBarrel(Barrel& barrel) const {
	return hasBarrelExploded(barrel) || barrel.getX() + barrel.getDirX() == board.getMinX() || barrel.getX() + barrel.getDirX() == board.getMaxX();
}

/**
 * @brief Handles explosions of barrels on the game board.
 */
void GameBase::explodeBarrels() {
	for (auto& barrel : barrels) {
		if (hasBarrelExploded(barrel)) {
			barrel.explode();
		}
	}
}

/**
 * @brief Deactivates barrels based on certain conditions.
 */
void GameBase::deactivateBarrels() {
	for (auto& barrel : barrels) {
		if (shouldDeactivateBarrel(barrel)) {
			barrel.deactivate();
		}
	}
}

/**
 * @brief Checks if a barrel has exploded.
 */
bool GameBase::hasBarrelExploded(Barrel& barrel) const {
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
void GameBase::checkGhostWithGhostCollisions() {
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
void GameBase::checkHammerPickUp() {
	if (mario.getPos() == board.getHammerPos() && !hammer) {
		hammer.emplace(mario);
		board.deleteHammer();
	}
}

/**
 * @brief Handles entity kills while the hammer is swinging.
 * Deactivates barrels and ghosts hit by the hammer, awarding points.
 */
void GameBase::checkKill() {
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
bool GameBase::isDirectCollision(const Entity& entity) const {
	return mario.getX() == entity.getX() && mario.getY() == entity.getY();
}

/**
 * @brief Checks if Mario narrowly missed a collision with a enemy entity.
 */
bool GameBase::isMissedCollision(const Entity& entity) const {
	int marioPreviousX = mario.getX() - mario.getDirX();
	int marioPreviousY = mario.getY() - mario.getDirY();
	return marioPreviousX == entity.getX() && marioPreviousY == entity.getY();
}

/**
 * @brief Checks if Mario is within the explosion radius of a barrel.
 */
bool GameBase::isInExplosionRadius(const Barrel& barrel) const {
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
bool GameBase::isExplosionFatal(const Barrel& barrel) const {
	return barrel.didExplode() && isInExplosionRadius(barrel);
}



// ------------------- Utility Functions -------------------

/**
 * @brief Moves Mario and barrels on the game board.
 */
void GameBase::moveCharacters() {
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
void GameBase::drawCharacters() {
	if (!silent) {
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
}

/**
 * @brief Displays Marios remaining lives.
 */
void GameBase::displayLives() {
	if (!silent) {
		int displayX = board.getLegendPos().getX();
		int displayY = board.getLegendPos().getY();
		gotoxy(displayX, displayY);
		std::cout << "LIVES: " << lives;
	}
}

/**
 * @brief Erases Mario and barrels from the game board.
 */
void GameBase::eraseCharacters() {
	if (!silent) {
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
}

/**
 * @brief Displays the current score on the game screen.
 */
void GameBase::displayScore() const {
	if (!silent) {
		int displayX = board.getLegendPos().getX();
		int displayY = board.getLegendPos().getY() + 1;

		gotoxy(displayX, displayY);
		std::cout << "SCORE: " << score;
	}
}




// ------------------- Stage Functions -------------------

/**
 * @brief Resets the stage for a new round.
 * Reinitializes barrels, Mario, ghosts, and game state variables.
 */
void GameBase::resetStage() {
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
	displayLives();
	eatBuffer();
}


// ------------------- Animation Functions -------------------

/**
 * @brief Makes Mario blink (disappear and reappear) visually.
 */
void GameBase::marioBlinkAnimation() {
	if (!silent) {
		for (int i = 0; i < blinkIterations; i++) {
			mario.draw();
			Sleep(200);
			mario.erase();
			Sleep(200);
		}
		eraseCharacters();
	}
}

/**
 * @brief Displays a visual effect for a hammer hit.
 */
void GameBase::hammerHitAnimation() {
	if (!silent) {
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
}

/**
 * @brief Animates the display of score points above Mario's position.
 * Checks valid positions and briefly shows the points before restoring the board.
 */
void GameBase::scoreAnimation(const std::string& points) {
	if (!silent) {
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
}

