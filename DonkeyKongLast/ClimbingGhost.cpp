#include "ClimbingGhost.h"

// ------------------- Action Functions -------------------

/**
 * @brief Handles the ghost's movement on the board.
 * The ghost changes direction if it encounters an invalid position or is not on a floor.
 * Random direction changes are introduced for dynamic behavior.
 */

void ClimbingGhost::move() {
	if (shouldTurnAround()) {
		turnAround();
	}
	else if (isOnLadder() && canStartClimbingUp()) {
		startClimbingUp();
	}
	else if (isClimbingUp && !isLadderAbove()) {
		stopClimbingUp();
	}
	else if (shouldStartClimbingDown()) {
		startClimbingDown();
	}
	else if (finishedClimbing()) {
		changeDirectionRandomly();
	}
	else if (!inCollision && !isClimbingUp && !isClimbingDown) {
		setRandDir();
	}

	Entity::move();
	inCollision = false;
}

void ClimbingGhost::collision() {
	turnAround();
	setDirY(-getDirY());
	if (isClimbingUp) {
		isClimbingDown = true;
		isClimbingUp = false;
	}
	else if (isClimbingDown) {
		isClimbingDown = false;
		isClimbingUp = true;
	}
	inCollision = true;
}

bool ClimbingGhost::shouldTurnAround() {
	return (!isNextPosFloor() || !isNextPositionValid()) && (!isClimbingDown && !isClimbingUp);
}

bool ClimbingGhost::canStartClimbingUp() {
	return !isClimbingDown && !isClimbingUp && getDirX() != X_NONE;
}

void ClimbingGhost::startClimbingUp() {
	setDirY(Y_UP);
	setDirX(X_NONE);
	isClimbingUp = true;
}

void ClimbingGhost::stopClimbingUp() {
	setDirY(Y_NONE);
	setY(getY() - 2);
	isClimbingUp = false;
}

bool ClimbingGhost::shouldStartClimbingDown() {
	return isLadderBelow() && isOnFloor() && getDirX() != X_NONE;
}

void ClimbingGhost::startClimbingDown() {
	setY(getY() + 1);
	setDirY(Y_DOWN);
	setDirX(X_NONE);
	isClimbingDown = true;
}

bool ClimbingGhost::finishedClimbing() {
	return getDirX() == X_NONE && !isClimbingUp && (!isClimbingDown || isOnFloor());
}

void ClimbingGhost::changeDirectionRandomly() {
	isClimbingDown = false;
	setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
}

