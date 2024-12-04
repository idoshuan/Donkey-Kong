#include "Mario.h"

void Mario::keyPressed(char key) {
	for (size_t k = 0; k < numKeys; k++) {
		if (std::tolower(key) == keys[k]) {
			setDir(getDir(k));
			if (k == 0) {
				handleUp();
			}
			else if (k == 1) {
				handleDown();
			}
			else {
				move();
			}
		}
	}
}

void Mario::handleUp() {
	if (!isJumping && !isFalling && !isClimbing) {
		if (isOnLadder()) {
			climb();
		}
		else {
			jump();
		}
	}
}

void Mario::handleDown() {
	return;
}

void Mario::jump() {
	return;
}

void Mario::climb() {
	return;
}


void Mario::fall() {
	return;
}
