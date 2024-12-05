#include "Mario.h"

void Mario::keyPressed(char key) {
	for (size_t k = 0; k < numKeys; k++) {
		if (std::tolower(key) == keys[k]) {
			if (k == 0) {
				handleUp();
				return;
			}
			else if (k == 1) {
				handleDown();
				return;
			}
			else {
				setDir(findDir(k));
				return;
			}
		}
	}
}

void Mario::move() {
	if (isClimbingUp || isClimbingDown) {
		climb();
	}
	else if (isJumping) {
		jump();
	}
	else if (isFalling) {
		fall();
	}
	Point::move();
}

void Mario::handleUp() {
	if (!isJumping && !isFalling && !isClimbingUp && !isClimbingDown) {
		setDirY(up);
		if (isOnLadder()) {
			setDirX(0);
			isClimbingUp = true;
		}
		else {
			isJumping = true;
		}
	}
}

void Mario::handleDown() {
	if (!isJumping && !isFalling && !isClimbingUp && !isClimbingDown) {
		setDirY(down);
		if (isLadder(getX(), getY() + 2)) {
			setY(getY() + 1);
		}
	}
}

void Mario::jump() {
	if (jumpCounter < jumpHeight && isNextPositionValid()) {
		jumpCounter++;
	}
	else {
		jumpCounter = 0;
		isJumping = false;
		isFalling = true;
		setDirY(down);
	}
}


void Mario::fall() {
	if (!isOnFloor()) {
		fallingCounter++;
	}
	else {
		fallingCounter = 0;
		isFalling = false;
		setDirY(0);
	}
}

void Mario::climb() {
	if (!isLadder(getX(), getY() + getDirY())) {
		if (isClimbingUp) {
			setY(getY() - 2);
			isClimbingUp = false;
		}
		else {
			isClimbingDown = false;
		}
		setDirY(0);
	}
	else {
		return;
	}
}