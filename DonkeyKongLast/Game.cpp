#include "Game.h"

void Game::startGame() {
    gameStartTime = clock::now();
    ShowConsoleCursor(false);
    while (lives > 0) {
        drawAndMoveCharacters();
        Sleep(100);
        checkForKeyPress();
        trySpawnBarrel();

        for (int i = 0; i < barrelCount; i++) {
            if (checkBarrelExplode(barrelArr[i])) {
                barrelArr[i].explode();
            }
        }

        if (checkMarioDeath()) {
            lives--;
            resetGame();
        }
        for (int i = 0; i < barrelCount; i++) {
            if (checkBarrelExplode(barrelArr[i])) {
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
            barrelArr[i].deactivate();
        }
        mario = Mario(marioInitX, marioInitY, &board);
        barrelCount = 0;
        firstBarrelSpawned = false;
        gameStartTime = clock::now();
        lastBarrelTime = gameStartTime;
}

void Game::marioBlink() {
    for (int i = 0; i < 6; i++) {
        mario.draw();
        Sleep(200);
        mario.erase();
        Sleep(200);
    }
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
        if (barrelArr[i].isCurrentlyActive()) {
          barrelArr[i].move();
          barrelArr[i].draw();
        }
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
            barrelArr[barrelCount] = Barrel(leftBarrelInitX, barrelInitY, &board);
            barrelArr[barrelCount].activate();
        }
        else {
            barrelArr[barrelCount] = Barrel(rightBarrelInitX, barrelInitY, &board);
            barrelArr[barrelCount].activate();
        }
        barrelCount++;
        lastBarrelTime = now;
    }
}

bool Game::checkMarioDeath() {
    return checkMarioDeathFromBarrel() || checkMarioDeathFromFall();
}
bool Game::isInExplosionRadius(Barrel& barrel) {
    const int explosionRadiusX = 5; // Horizontal radius (left and right)
    const int explosionRadiusY = 3; // Vertical radius (above)

    int barrelX = barrel.getX();
    int barrelY = barrel.getY();

    int marioX = mario.getX();
    int marioY = mario.getY();

    // Check if Mario's position is within the explosion radius
    return (marioX >= barrelX - explosionRadiusX && marioX <= barrelX + explosionRadiusX &&
        marioY >= barrelY - explosionRadiusY && marioY <= barrelY);
}



bool Game::checkMarioDeathFromBarrel() {
    for (int i = 0; i < barrelCount; i++)
    {
        if (barrelArr[i].isCurrentlyActive())
        {
            if (mario.getX() == barrelArr[i].getX() && mario.getY() == barrelArr[i].getY()) {
                return true;
            }
            if (barrelArr[i].didExplode() && isInExplosionRadius(barrelArr[i])) {
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

bool Game::checkBarrelExplode(Barrel &barrel) {
        if (barrel.getFallingCounter() >= barrelMaxFallHeight && !barrel.isCurrentlyFalling()) {
            return true;
        }
        else {
            barrel.resetFallingCounterIfNeeded();
            return false;
        }
}