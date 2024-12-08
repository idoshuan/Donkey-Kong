#include "Barrel.h"
// TODO change magic numbers


void Barrel::move() {
	if (floorDir() == '<') {
		setDirX(-1);
	}
	else if (floorDir() == '>') {
		setDirX(1);
	}
	if (!isOnFloor() || isFalling) {
		fall();
	}
	Point::move();
}

void Barrel::fall() {
	isFalling = true;
	setDirY(down);
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