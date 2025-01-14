#include "Ghost.h"

void Ghost::move() {
	if (!isNextPosFloor()) {
		turnAround();
	}
	else if(!inCollision){
		setRandDir(); // Need to change to flag so when collision random doesnt start
	}
	Entity::move();
	inCollision = false;
}

void Ghost::setRandDir() {
	if (getRandomDouble() <= 0.05) {
		turnAround();
	}
}


