#include "Game.h"

// ------------------- Game Loop -------------------

/**
 * @brief Starts the main game loop.
 * Continuously runs while the game is active (`isRunning`) and delegates control to `handleGameState()`.
 */
void Game::startGame() {
    ShowConsoleCursor(false);
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
        gameStartTime = clock::now();
        board.print();
        displayLives();
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
    drawCharacters();
    Sleep(70);
    checkForKeyPress();
    eraseCharacters();
    trySpawnBarrel();
    explodeBarrels();
    if (checkMarioDeath()) {
        lives--;
        resetStage();
        return;
    }
    if (checkMarioWon()) {
        gameState = GameState::WON;
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
    marioBlink();

    for (int i = 0; i < maxBarrels; i++) {
        barrelArr[i] = Barrel();
    }
    mario = Mario(marioInitPos, &board);
    barrelCount = 0;
    firstBarrelSpawned = false;
    gameStartTime = clock::now();
    lastBarrelTime = gameStartTime;
    displayLives();
    while (_kbhit()) { // Clean the buffer after mario dies
        _getch();
    }
}

// ------------------- Input Handling -------------------

/**
 * @brief Checks for user input during gameplay.
 * Processes ESC to pause the game or sends key input to Mario.
 */
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

/**
 * @brief Handles menu actions like starting the game, showing instructions, or exiting.
 */
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

// ------------------- Mario-Related Functions -------------------

/**
 * @brief Checks if Mario has died due to a barrel or falling.
 * Combines checks for barrel collisions and falling beyond the maximum height.
 */
bool Game::checkMarioDeath() {
    return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}

/**
 * @brief Checks if Mario has reached the goal (Paulina).
 * Compares Mario's position with Paulina's position.
 */
bool Game::checkMarioWon() {
    Point paulina = board.getPaulina();
    return mario.getX() + mario.getDirX() == paulina.getX() && mario.getY() == paulina.getY();
}

/**
 * @brief Checks if Mario has died from a barrel collision.
 */
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
 * @brief Makes Mario blink (disappear and reappear) visually.
 */
void Game::marioBlink() {
    drawCharacters();
    for (int i = 0; i < 6; i++) {
        mario.draw();
        Sleep(200);
        mario.erase();
        Sleep(200);
    }
    eraseCharacters();
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
    if (!firstBarrelSpawned || barrelCount >= maxBarrels) return false;
    auto timeSinceLastBarrelSpawn = std::chrono::duration_cast<milliseconds>(now - lastBarrelTime).count();
    return timeSinceLastBarrelSpawn >= barrelSpawnInterval;
}

/**
 * @brief Spawns a new barrel at the left or right position.
 * Activates the newly created barrel and increments the barrel count.
 */
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
    for (int i = 0; i < barrelCount; i++) {
        if (hasBarrelExploded(barrelArr[i])) {
            barrelArr[i].explode();
        }
    }
}

/**
 * @brief Deactivates barrels based on certain conditions.
 */
void Game::deactivateBarrels() {
    for (int i = 0; i < barrelCount; i++) {
        if (shouldDeactivateBarrel(barrelArr[i])) {
            barrelArr[i].deactivate();
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

// ------------------- Collision and Explosion Checks -------------------

/**
 * @brief Checks if Mario is within the explosion radius of a barrel.
 */
bool Game::isInExplosionRadius(const Barrel& barrel) const {
    int barrelX = barrel.getX();
    int barrelY = barrel.getY();

    int marioX = mario.getX();
    int marioY = mario.getY();

    int difX = std::abs(marioX - barrelX);
    int difY = marioY - barrelY;

    return (difX <= barrelExplosionRadius && difY <= 0 && difY <= barrelExplosionRadius);
}

/**
 * @brief Checks for a direct collision between Mario and a barrel.
 */
bool Game::isDirectCollision(const Barrel& barrel) const {
    return mario.getX() == barrel.getX() && mario.getY() == barrel.getY();
}

/**
 * @brief Checks if Mario narrowly missed a collision with a barrel.
 */
bool Game::isMissedCollision(const Barrel& barrel) const {
    int marioPreviousX = mario.getX() - mario.getDirX();
    int marioPreviousY = mario.getY() - mario.getDirY();
    return marioPreviousX == barrel.getX() && marioPreviousY == barrel.getY();
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

/**
 * @brief Displays the pause screen with instructions to resume.
 */
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

/**
 * @brief Clears the pause screen.
 */
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

/**
 * @brief Moves Mario and barrels on the game board.
 */
void Game::moveCharacters() {
    mario.move();
    for (int i = 0; i < barrelCount; i++) {
        if (barrelArr[i].isCurrentlyActive()) {
            barrelArr[i].move();
        }
    }
}

/**
 * @brief Draws Mario and barrels on the game board.
 */
void Game::drawCharacters() {
    mario.draw();
    for (int i = 0; i < barrelCount; i++) {
        if (barrelArr[i].isCurrentlyActive()) {
            barrelArr[i].draw();
        }
    }
}

/**
 * @brief Erases Mario and barrels from the game board.
 */
void Game::eraseCharacters() {
    mario.erase();
    for (int i = 0; i < barrelCount; i++) {
        barrelArr[i].erase();
    }
}


/**
 * @brief Handles the actions when the player wins the game.
 * Resets the game and displays the "You Win" screen.
 */
void Game::handleGameWin() {
    lives = initLives;
    resetStage();
    screen.printWinScreen();
    Sleep(3500); // Pause to allow the player to see the screen
    gameState = GameState::MENU;
}

/**
 * @brief Handles the actions when the player loses the game.
 * Resets the game and displays the "Game Over" screen.
 */
void Game::handleGameOver() {
    lives = initLives;
    screen.printLoseScreen();
    Sleep(3500); // Pause to allow the player to see the screen
    gameState = GameState::MENU;
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