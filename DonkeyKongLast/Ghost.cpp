#include "Ghost.h"

void Ghost::move() {
	if (!isNextPosFloor()) {
		turnAround();
	}
	else {
		setRandDir(); // Need to change to flag so when collision random doesnt start
	}
	Entity::move();
}

void Ghost::setRandDir() {
	if (getRandomDouble() <= 0.05) {
		turnAround();
	}
}


