#include "ClimbingGhost.h"

// ------------------- Action Functions -------------------

/**
 * @brief Handles the ghost's movement on the board.
 * The ghost changes direction if it encounters an invalid position or is not on a floor.
 * Random direction changes are introduced for dynamic behavior.
 */
void ClimbingGhost::move() {
	if (!isNextPosFloor() || !getBoard()->isValidPosition(getNextPos()) && !isClimbingDown && !isClimbingUp) {
		turnAround();
	}
	else if (isOnLadder() && !isClimbingDown && !isClimbingUp) {
		setDirY(Y_UP);
		setDirX(X_NONE);
		isClimbingUp = true;
		if (isOnFloor() && !isOnLadder()) {
			setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
			isClimbingUp = false;
		}
	}
	else if (getBoard()->isLadder(Point(getX(), getY() + 2)) && !isOnLadder()) {
		setDirY(Y_DOWN);
		setDirX(X_NONE);
		isClimbingDown = true;
		if (isOnFloor()) {
			setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
			isClimbingDown = false;
		}
	}
	else if (!inCollision && !isClimbingUp && !isClimbingDown) {
		setRandDir();
	}
	Entity::move();
	inCollision = false;
}

/**
 * @brief Randomly changes the ghost's direction.
 * Introduces a 0.05 probability for the ghost to turn around.
 */
void ClimbingGhost::setRandDir() {
	if (getRandomDouble() <= 0.05) {
		turnAround();
	}
}



