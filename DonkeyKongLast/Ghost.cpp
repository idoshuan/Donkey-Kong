#include "Ghost.h"

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

void Ghost::setRandDir() {
	if (getRandomDouble() <= 0.05) {
		turnAround();
	}
}


