#include "Barrel.h"
// TODO change magic numbers
// Is resetfalling counter if needed a good option or is better to handle explosion logic inside class

void Barrel::move() {
	if (floorDir() == '<') {
		setDirX(-1);
		prevDirX = -1;
	}
	else if (floorDir() == '>') {
		setDirX(1);
		prevDirX = 1;
	}
	else if (floorDir() == '=') {
		setDirX(prevDirX);
	}
	if (!isOnFloor() || isFalling) {
		fall();
	}
	Entity::move();
}

void Barrel::fall() {
	isFalling = true;
	setDir(Entity::DOWN);
	if (!isOnFloor()) {
		fallingCounter++;
	}
	else {
		resetFallingCounter = true;
		isFalling = false;
		setDirY(0);
	}
}

void Barrel::resetFallingCounterIfNeeded() {
	if (resetFallingCounter) {
		fallingCounter = 0;
		resetFallingCounter = false;
	}
}