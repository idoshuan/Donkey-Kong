#include "Game.h"\

void Game::startGame() {
    gameStartTime = clock::now();
    ShowConsoleCursor(false);
    while (lives > 0) {
        drawAndMoveCharacters();
        Sleep(100);
        checkForKeyPress();
        trySpawnBarrel();
        if (checkMarioDeath()) {
            lives--;
            resetGame();
        }
        eraseCharacters();
    }
}

void Game::resetGame() {
        for (int i = 0; i < 6; i++) {
            mario.draw();
            Sleep(200);
            mario.erase();
            Sleep(200);
        }
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

void Game::checkForKeyPress() {
    if (_kbhit()) {
        char key = _getch();
        if (key == ESC) {
            return;
        }
        mario.keyPressed(key);
    }
}

void Game::eraseCharacters() {
    mario.erase();
    for (int i = 0; i < barrelCount; i++) {
        barrelArr[i].erase();
    }
}

void Game::drawAndMoveCharacters() {
    mario.move();
    mario.draw();
    for (int i = 0; i < barrelCount; i++) {
        barrelArr[i].move();
        barrelArr[i].draw();
    }
}

void Game::trySpawnBarrel() {
    auto now = clock::now();
    auto timeSinceLastBarrelSpawn = std::chrono::duration_cast<milliseconds>(now - lastBarrelTime).count();

    if (!firstBarrelSpawned) {
        auto gameElapsedTime = std::chrono::duration_cast<milliseconds>(now - gameStartTime).count();
        if (gameElapsedTime > firstBarrelSpawnDelay) {
            firstBarrelSpawned = true;
        }
    }

    if (firstBarrelSpawned && timeSinceLastBarrelSpawn >= barrelSpawnInterval && barrelCount < numBarrels) {
        if (barrelCount % 2 == 0) {
            barrelArr[barrelCount++] = Barrel(leftBarrelInitX, barrelInitY, &board);
        }
        else {
            barrelArr[barrelCount++] = Barrel(rightBarrelInitX, barrelInitY, &board);
        }
        lastBarrelTime = now;
    }
}

bool Game::checkMarioDeath() {
    return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}

bool Game::checkMarioDeathFromBarrel() {
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
