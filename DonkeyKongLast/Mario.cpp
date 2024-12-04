#include "Mario.h"

void Mario::keyPressed(char key) {
	for (size_t k = 0; k < numKeys; k++) {
		if (std::tolower(key) == keys[k]) {
			if (k == 0) {
				handleUp();
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
	if (isClimbing) {
		climb();
	}
	else if (isJumping) {
		setDirY(up);
		jump();
	}
	else if (isFalling) {
		fall();
	}
	Point::move();
}

void Mario::handleUp() {
	if (!isJumping && !isFalling && !isClimbing) {
		if (isOnLadder()) {
			isClimbing = true;
		}
		else {
			isJumping = true;
		}
	}
}

void Mario::handleDown() {
	return;
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

void Mario::climb() {
	
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
