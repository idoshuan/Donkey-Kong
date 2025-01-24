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

/**
 * @brief Handles the ghost's collision with other ghosts.
 * The ghost turns around and adjusts its vertical direction.
 * If it is climbing up, it starts climbing down, and vice versa.
 */
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

/**
 * @brief Determines if the ghost should turn around.
 * The ghost will turn around if it encounters an invalid position
 * or is not on a floor, provided it is not climbing up or down.
 */
bool ClimbingGhost::shouldTurnAround() {
	return (!isNextPosFloor() || !isNextPositionValid()) && (!isClimbingDown && !isClimbingUp);
}

/**
 * @brief Checks if the ghost can start climbing up.
 */
bool ClimbingGhost::canStartClimbingUp() {
	return !isClimbingDown && !isClimbingUp && getDirX() != X_NONE;
}

/**
 * @brief Starts climbing up if conditions are met.
 */
void ClimbingGhost::startClimbingUp() {
	if (getRandomDouble() >= 0.5) {
		setDirY(Y_UP);
		setDirX(X_NONE);
		isClimbingUp = true;
	}
}

/**
 * @brief Stops climbing up and adjusts position.
 */
void ClimbingGhost::stopClimbingUp() {
	setDirY(Y_NONE);
	setY(getY() - 2);
	isClimbingUp = false;
}

/**
 * @brief Checks if the ghost should start climbing down.
 */
bool ClimbingGhost::shouldStartClimbingDown() {
	return isLadderBelow() && isOnFloor() && getDirX() != X_NONE;
}

/**
 * @brief Starts climbing down if conditions are met.
 */
void ClimbingGhost::startClimbingDown() {
	if (getRandomDouble() >= 0.5) {
		setY(getY() + 1);
		setDirY(Y_DOWN);
		setDirX(X_NONE);
		isClimbingDown = true;
	}
}

/**
 * @brief Checks if the ghost has finished climbing.
 */
bool ClimbingGhost::finishedClimbing() {
	return getDirX() == X_NONE && !isClimbingUp && (!isClimbingDown || isOnFloor());
}

/**
 * @brief Randomly changes the ghost's horizontal direction when finished climbing.
 */
void ClimbingGhost::changeDirectionRandomly() {
	isClimbingDown = false;
	setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
}

