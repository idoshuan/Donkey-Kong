#include "Mario.h"

void Mario::keyPressed(char key) {
	char keyPressed = std::tolower(key);
	if (isValidKey(key)) {
		switch (keyPressed) {
		case 'w':
			handleUp();
			return;
		case 'x':
			handleDown();
			return;
		case 'a':
			handleHorizontal(keyPressed);
			return;
		case 'd':
			handleHorizontal(keyPressed);
			return;
		case 's':
			isClimbingUp = isClimbingDown = false;
			setDir(Point::NONE);
			return;
		default:
			return;
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
	else if (isFalling || !isOnFloor()) {
		fall();
	}
	Point::move();
}

void Mario::handleUp() {
	if (!isJumping && !isFalling) {
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
	if (!isJumping && !isFalling) {
		setDir(Point::DOWN);
		if (isLadder(getX(), getY() + 2) && !isOnLadder()) {
			setY(getY() + 1);
		}
		setDirX(0);
		isClimbingDown = true;
	}
}

void Mario::handleHorizontal(char keyPressed) {
	if (!isClimbingDown && !isClimbingUp) {
		if (!isOnFloor()) {
			isFalling = true;
		}
		else {
			switch (keyPressed) {
			case 'a':
				setDir(Point::LEFT);
				break;
			case 'd':
				setDir(Point::RIGHT);
				break;
			default:
				return;
			}
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

void Mario::climb() {
	if (isLadder(getX(), getY() + getDirY())) {
		return;
	}
	else {
		if (isClimbingUp) {
			setY(getY() - 2);
			isClimbingUp = false;
		}
		else {
			isClimbingDown = false;
		}
		setDirY(0);
	}
}