#include "Game.h"

// ------------------- Game Loop -------------------
void Game::startGame() {
	ShowConsoleCursor(false);
	while (isRunning) {
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
	case GameState::GAME_OVER:
		handleGameOver();
		break;;
	case GameState::WON:
		handleGameWin();
		break;
	default:
		std::cerr << "Unknown game state!" << std::endl;
		isRunning = false;
	}
}

void Game::updateGameLogic() {
	displayLives();
	drawCharacters();
	Sleep(70);
	checkForKeyPress();
	eraseCharacters();
	trySpawnBarrel();
	explodeBarrels();
	if (checkMarioDeath()) {
		lives--;
		resetGame();
		return;
	}
	if (checkMarioWon()) {
		gameState = GameState::WON;
		return;
	}
	deactivateBarrels();
	moveCharacters();
}

void Game::resetGame() {
	marioBlink();
	eraseCharacters();

	for (int i = 0; i < numBarrels; i++) {
		barrelArr[i] = Barrel();
	}
	mario = Mario(marioInitPos, &board);
	barrelCount = 0;
	firstBarrelSpawned = false;
	gameStartTime = clock::now();
	lastBarrelTime = gameStartTime;
}



// ------------------- Input Handling -------------------
void Game::checkForKeyPress() {
	if (_kbhit()) {
		KEYS key = charToKey(_getch());
		if (key == KEYS::ESC) {
			gameState = GameState::PAUSED;
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
		isRunning = false;
		break;
	}
}

// ------------------- Mario-related Functions -------------------
bool Game::checkMarioDeath() {
	return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}
bool Game::checkMarioWon() {
	Point paulina = board.getPaulina();
	return mario.getX() + mario.getDirX() == paulina.getX() && mario.getY() == paulina.getY();
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

void Game::marioBlink() {
	for (int i = 0; i < 6; i++) {
		mario.draw();
		Sleep(200);
		mario.erase();
		Sleep(200);
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
		barrelArr[barrelCount] = Barrel(leftBarrelPos, &board);
	}
	else {
		barrelArr[barrelCount] = Barrel(rightBarrelPos, &board);
	}
	barrelArr[barrelCount].activate();
	barrelCount++;
}

bool Game::shouldDeactivateBarrel(Barrel& barrel) const {
	return hasBarrelExploded(barrel) || barrel.getX() == board.getMinX() || barrel.getX() == board.getMaxX();
}


void Game::explodeBarrels() {
	for (int i = 0; i < barrelCount; i++) {
		if (hasBarrelExploded(barrelArr[i])) {
			barrelArr[i].explode();
		}
	}
}

void Game::deactivateBarrels() {
	for (int i = 0; i < barrelCount; i++) {
		if (shouldDeactivateBarrel(barrelArr[i])) {
			barrelArr[i].deactivate();
		}
	}
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
void Game::handlePause() {

	if (!isAlreadyPaused) {
		displayPauseScreen();
		drawCharacters();
		isAlreadyPaused = true;
	}

	if (_kbhit()) {
		char key = _getch();
		if (key == KEYS::ESC) {
			clearPauseScreen();
			isAlreadyPaused = false;
			gameState = GameState::PLAYING;
		}
	}
}


void Game::displayPauseScreen() {
	gotoxy(pauseMessageX, pauseMessageY);
	std::cout << "----------------------------";
	gotoxy(pauseMessageX, pauseMessageY + 1);
	std::cout << "        GAME PAUSED         ";
	gotoxy(pauseMessageX, pauseMessageY + 2);
	std::cout << " Press ESC again to resume  ";
	gotoxy(pauseMessageX, pauseMessageY + 3);
	std::cout << "----------------------------";
}

void Game::clearPauseScreen() {
	int startX = pauseMessageX;
	int startY = pauseMessageY;
	int width = pauseMessageWidth;
	int height = pauseMessageHeight;

	for (int y = 0; y < height; ++y) {
		gotoxy(startX, startY + y);
		for (int x = 0; x < width; ++x) {
			std::cout << ' ';
		}
	}
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

void Game::displayLives() const {
	int displayX = 6;
	int displayY = 0;

	gotoxy(displayX, displayY);

	std::cout << lives;
}

void Game::handleGameWin() {
	resetGame();
	clearScreen();

	std::cout << R"(
      .--..--..--..--..--..--..--..--..--..--..--..--..--..--. 
	  / .. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \
	  \ \/\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ \/ /
	   \/ /`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'\/ / 
	   / /\                                                / /\ 
	  / /\ \   __   __           __        ___       _    / /\ \
	  \ \/ /   \ \ / /__  _   _  \ \      / (_)_ __ | |   \ \/ /
	   \/ /     \ V / _ \| | | |  \ \ /\ / /| | '_ \| |    \/ / 
       / /\      | | (_) | |_| |   \ V  V / | | | | |_|    / /\ 
	  / /\ \     |_|\___/ \__,_|    \_/\_/  |_|_| |_(_)   / /\ \
	  \ \/ /                                              \ \/ /
	   \/ /                                                \/ / 
	   / /\.--..--..--..--..--..--..--..--..--..--..--..--./ /\ 
	  / /\ \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \.. \/\ \
	  \ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `'\ `' /
	   `--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--'`--' 
)" << std::endl;
	Sleep(1500);
	gameState = GameState::MENU;
}

void Game::handleGameOver()
{
	lives = initLives;
	clearScreen();
	std::cout << R"(
  __     ______  _    _   _      ____   _____ ______ 
  \ \   / / __ \| |  | | | |    / __ \ / ____|  ____|
   \ \_/ / |  | | |  | | | |   | |  | | (___ | |__   
    \   /| |  | | |  | | | |   | |  | |\___ \|  __|  
     | | | |__| | |__| | | |___| |__| |____) | |____ 
     |_|  \____/ \____/  |______\____/|_____/|______|
                                                    
)" << std::endl;
	Sleep(1500);
	gameState = GameState::MENU;
}
