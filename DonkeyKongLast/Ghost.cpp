#include "Ghost.h"

// ------------------- Action Functions -------------------

/**
 * @brief Handles the ghost's movement on the board.
 * The ghost changes direction if it encounters an invalid position or is not on a floor.
 * Random direction changes are introduced for dynamic behavior.
 */
void Ghost::move() {
	if (!isNextPosFloor()||!getBoard()->isValidPosition(getNextPos())) {
		turnAround();
	}
	else if(!inCollision){
		setRandDir(); 
	}
	Entity::move();
	inCollision = false;
}

/**
 * @brief Randomly changes the ghost's direction.
 * Introduces a 0.05 probability for the ghost to turn around.
 */
void Ghost::setRandDir() {
	if (getRandomDouble() <= 0.05) {
		turnAround();
	}
}


