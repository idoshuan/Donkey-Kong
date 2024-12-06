#include "Barrel.h"

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
		fallingCounter = 0;
		isFalling = false;
		setDirY(0);
	}
}