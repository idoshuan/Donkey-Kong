#include "ClimbingGhost.h"

// ------------------- Action Functions -------------------

/**
 * @brief Handles the ghost's movement on the board.
 * The ghost changes direction if it encounters an invalid position or is not on a floor.
 * Random direction changes are introduced for dynamic behavior.
 */
void ClimbingGhost::move() {
	if ((!isNextPosFloor() || !getBoard()->isValidPosition(getNextPos())) && !isClimbingDown && !isClimbingUp) {
		turnAround();
	}
	else if (isOnLadder() && !isClimbingDown && !isClimbingUp) {
		setDirY(Y_UP);
		setDirX(X_NONE);
		isClimbingUp = true;
	}
	else if (isClimbingUp && !getBoard()->isLadder(Point(getX(), getY() + getDirY()))) {
		setY(getY() - 2);
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
		setDirY(Y_NONE);
		isClimbingUp = false;
	}
	else if (getBoard()->isLadder(Point(getX(), getY() + 2)) && isOnFloor()) {
		setY(getY() + 1);
		setDirY(Y_DOWN);
		setDirX(X_NONE);
		isClimbingDown = true;
	}
	else if (isClimbingDown && isOnFloor()) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
		setDirY(Y_NONE);
		isClimbingDown = false;
	}
	else if (!inCollision && !isClimbingUp && !isClimbingDown) {
		setRandDir();
	}
	Entity::move();
	inCollision = false;
}



