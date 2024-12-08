#include "Barrel.h"
// TODO change magic numbers


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
	Point::move();
}

void Barrel::fall() {
	isFalling = true;
	setDir(Point::DOWN);
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