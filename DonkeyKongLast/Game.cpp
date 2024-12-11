#include "Game.h"

// ------------------- Constructor -------------------
Game::Game()
	:gameState(GameState::MENU),
	mario(marioInitX, marioInitY, &board),
	lives(initLives),
	barrelCount(0),
	firstBarrelSpawned(false) {
}

// ------------------- Game Loop -------------------
void Game::startGame() {
	ShowConsoleCursor(false);
	while (true) {
		handleGameState();
	}
}

void Game::handleGameState() {
	switch (gameState) {
	case GameState::MENU:
		menu.displayMenu();
		handleMenuState(menu.getAction());
		break;
	case GameState::START:
		gameStartTime = clock::now();
		board.print();
		gameState = GameState::PLAYING;
		break;
	case GameState::PLAYING:
		if (lives > 2) {
			updateGameLogic();
			checkForKeyPress();
		}
		else {
			gameState = GameState::GAME_OVER;
		}
		break;
	case GameState::PAUSED:
		handlePauseInput();
		break;

	case GameState::GAME_OVER:
		handleGameOver();
		return;
	case GameState::WON:
		handleGameWin();
		return;
	default:
		std::cerr << "Unknown game state!" << std::endl;
		return startGame();
	}
}

void Game::handleGameWin(){
	clearScreen();

	std::cout << R"(
  __     ______  _    _   ___         _______  _   _ 
  \ \   / / __ \| |  | |  \ \        / /_   _|| \ | |
   \ \_/ / |  | | |  | |   \ \  /\  / /  | |  |  \| |
    \   /| |  | | |  | |    \ \/  \/ /   | |  | . ` |
     | | | |__| | |__| |     \  /\  /   _| |_ | |\  |
     |_|  \____/ \____/       \/  \/   |_____||_| \_|
                                                    
    )";	Sleep(1000);
	gameState = GameState::MENU;
}

void Game::handleGameOver()
{
	marioBlink();

	clearScreen(); 
	std::cout << R"(
  __     ______  _    _   _      ____   _____ ______ 
  \ \   / / __ \| |  | | | |    / __ \ / ____|  ____|
   \ \_/ / |  | | |  | | | |   | |  | | (___ | |__   
    \   /| |  | | |  | | | |   | |  | |\___ \|  __|  
     | | | |__| | |__| | | |___| |__| |____) | |____ 
     |_|  \____/ \____/  |______\____/|_____/|______|
                                                    
)" << std::endl;
	Sleep(1000);
	gameState = GameState::MENU;
}
void Game::updateGameLogic() {
	drawCharacters();
	Sleep(100);
	eraseCharacters();

	trySpawnBarrel();

	for (int i = 0; i < barrelCount; i++) {
		if (hasBarrelExploded(barrelArr[i])) {
			barrelArr[i].explode();
		}
	}

	if (checkMarioDeath()) {
		lives--;
		resetGame();
		return;
	}
	if (checkMarioWon()) {
		gameState = GameState::WON;
	}
	for (int i = 0; i < barrelCount; i++) {
		if (shouldDeactivateBarrel(barrelArr[i])) {
			barrelArr[i].deactivate();
		}
	}
	moveCharacters();

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
			gameState = GameState::PAUSED;
			drawCharacters();
			return;
		}
		mario.keyPressed(key);
	}
}

void Game::handleMenuState(MenuAction action) {
	switch (action) {
	case MenuAction::START_GAME:
		gameState = GameState::START;
		break;
	case MenuAction::SHOW_INSTRUCTIONS:
		menu.displayInstructions();
		break;
	case MenuAction::EXIT_GAME:
		exit(0);
	}
}

// ------------------- Mario-related Functions -------------------
bool Game::checkMarioDeath() {
	return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}
bool Game::checkMarioWon() {
	return mario.getX() == 38 && mario.getY() == 0;
}

void Game::marioBlink() {
	for (int i = 0; i < 6; i++) {
		mario.draw();
		Sleep(200);
		mario.erase();
		Sleep(200);
	}
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

// ------------------- Barrel-related Functions -------------------
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
	int marioPreviousY = mario.getY() - mario.getDirY();
	return marioPreviousX == barrel.getX() && marioPreviousY == barrel.getY();
}

bool Game::isExplosionFatal(const Barrel& barrel) const {
	return barrel.didExplode() && isInExplosionRadius(barrel);
}

// ------------------- Pause Handling -------------------
void Game::handlePauseInput() {
	if (_kbhit()) {
		char key = _getch();
		if (key == ESC) {
				gameState = GameState::PLAYING; // Resume the game
		}
	}
}

void Game::displayPauseScreen() {
	std::cout << "\n\n\n";
	std::cout << "----------------------------\n";
	std::cout << "        GAME PAUSED         \n";
	std::cout << " Press ESC again to resume  \n";
	std::cout << "----------------------------\n";
}

// ------------------- Utility Functions -------------------

void Game::moveCharacters() {
	mario.move();
	for (int i = 0; i < barrelCount; i++) {
		if (barrelArr[i].isCurrentlyActive()) {
			barrelArr[i].move();
		}
	}
}

void Game::drawCharacters() {
	mario.draw();
	for (int i = 0; i < barrelCount; i++) {
		if (barrelArr[i].isCurrentlyActive()) {
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
