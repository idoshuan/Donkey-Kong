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



