#include "Game.h"

// ------------------- Constructor -------------------
Game::Game()
	: mario(marioInitX, marioInitY, &board),
	lives(initLives),
	barrelCount(0),
	firstBarrelSpawned(false) {
}

// ------------------- Game Loop -------------------
void Game::startGame() {
	gameStartTime = clock::now();
	ShowConsoleCursor(false);

	while (lives > 0) {
		drawAndMoveCharacters();
		Sleep(100);
		checkForKeyPress();
		trySpawnBarrel();

		for (int i = 0; i < barrelCount; i++) {
			if (hasBarrelExploded(barrelArr[i])) {
				barrelArr[i].explode();
			}
		}

		if (checkMarioDeath()) {
			lives--;
			resetGame();
			break;
		}

		for (int i = 0; i < barrelCount; i++) {
			if (shouldDeactivateBarrel(barrelArr[i])) {
				barrelArr[i].deactivate();
			}
		}

		eraseCharacters();
	}
}

void Game::resetGame() {
	marioBlink();
	eraseCharacters();

	for (int i = 0; i < numBarrels; i++) {
		barrelArr[i] = Barrel();
	}
	mario = Mario(marioInitX, marioInitY, &board);
	barrelCount = 0;
	firstBarrelSpawned = false;
	gameStartTime = clock::now();
	lastBarrelTime = gameStartTime;
}

// ------------------- Input Handling -------------------
void Game::checkForKeyPress() {
	if (_kbhit()) {
		char key = _getch();
		if (key == ESC) {
			return;
		}
		mario.keyPressed(key);
	}
}

// ------------------- Drawing and Movement -------------------
void Game::drawAndMoveCharacters() {
	mario.move();
	mario.draw();

	for (int i = 0; i < barrelCount; i++) {
		if (barrelArr[i].isCurrentlyActive()) {
			barrelArr[i].move();
			barrelArr[i].draw();
		}
	}
}

void Game::eraseCharacters() {
	mario.erase();
	for (int i = 0; i < barrelCount; i++) {
		barrelArr[i].erase();
	}
}

// ------------------- Barrel Management -------------------
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

bool Game::shouldSpawnFirstBarrel(const time& now) const {
	if (firstBarrelSpawned) return false;
	auto gameElapsedTime = std::chrono::duration_cast<milliseconds>(now - gameStartTime).count();
	return gameElapsedTime > firstBarrelSpawnDelay;
}

bool Game::canSpawnBarrel(const time& now) const {
	if (!firstBarrelSpawned || barrelCount >= numBarrels) return false;
	auto timeSinceLastBarrelSpawn = std::chrono::duration_cast<milliseconds>(now - lastBarrelTime).count();
	return timeSinceLastBarrelSpawn >= barrelSpawnInterval;
}

void Game::spawnBarrel() {
	if (barrelCount % 2 == 0) {
		barrelArr[barrelCount] = Barrel(leftBarrelInitX, barrelInitY, &board);
	}
	else {
		barrelArr[barrelCount] = Barrel(rightBarrelInitX, barrelInitY, &board);
	}
	barrelArr[barrelCount].activate();
	barrelCount++;
}

bool Game::shouldDeactivateBarrel(Barrel& barrel) const {
	return hasBarrelExploded(barrel) || barrel.getX() == board.getMinX() || barrel.getX() == board.getMaxX();
}

bool Game::hasBarrelExploded(Barrel& barrel) const {
	if (barrel.getFallingCounter() >= barrelMaxFallHeight && !barrel.isCurrentlyFalling()) {
		return true;
	}
	else {
		barrel.resetFallingCounterIfNeeded();
		return false;
	}
}

// ------------------- Death Checks -------------------
bool Game::checkMarioDeath() {
	return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}

bool Game::checkMarioDeathFromBarrel() {
	for (int i = 0; i < barrelCount; i++) {
		Barrel barrel = barrelArr[i];
		if (barrel.isCurrentlyActive()) {
			if (isDirectCollision(barrel) || isMissedCollision(barrel) || isExplosionFatal(barrel)) {
				return true;
			}
		}
	}
	return false;
}

bool Game::checkMarioDeathFromFall() {
	if (mario.getFallingCounter() >= marioMaxFallHeight && !mario.isCurrentlyFalling()) {
		return true;
	}
	else {
		mario.resetFallingCounterIfNeeded();
		return false;
	}
}

// ------------------- Collision and Explosion Checks -------------------
bool Game::isInExplosionRadius(const Barrel& barrel) const {
	int barrelX = barrel.getX();
	int barrelY = barrel.getY();

	int marioX = mario.getX();
	int marioY = mario.getY();

	int difX = std::abs(marioX - barrelX);
	int difY = marioY - barrelY;

	return (difX <= explosionRadius && difY <= 0 && difY <= explosionRadius);
}

bool Game::isDirectCollision(const Barrel& barrel) const {
	return mario.getX() == barrel.getX() && mario.getY() == barrel.getY();
}

bool Game::isMissedCollision(const Barrel& barrel) const {
	int marioPreviousX = mario.getX() - mario.getDirX();
	return marioPreviousX == barrel.getX() && mario.getY() == barrel.getY();
}

bool Game::isExplosionFatal(const Barrel& barrel) const {
	return barrel.didExplode() && isInExplosionRadius(barrel);
}

// ------------------- Mario Effects -------------------
void Game::marioBlink() {
	for (int i = 0; i < 6; i++) {
		mario.draw();
		Sleep(200);
		mario.erase();
		Sleep(200);
	}
}
