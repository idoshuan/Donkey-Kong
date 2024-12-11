#include "Mario.h"

// fix stay while falling
// ask amir about line 62

void Mario::keyPressed(KEYS keyPressed) {
	if (isValidKey(keyPressed)) {
		switch (keyPressed) {
		case KEYS::UP:
			handleUp();
			return;
		case KEYS::DOWN:
			handleDown();
			return;
		case KEYS::LEFT:
		case KEYS::RIGHT:
			handleHorizontal(keyPressed);
			return;
		case KEYS::STAY:
			setDir(Entity::NONE);
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
	Entity::move();
}

void Mario::handleUp() {
	if (!isJumping && !isFalling) {
		setDirY(UP);
		if (isOnLadder()) {
			setDirX(STAY);
			isClimbingUp = true;
		}
		else {
			isJumping = true;
		}
	}
}

void Mario::handleDown() {
	if (!isJumping && !isFalling) {
		setDir(Entity::DOWN);
		isClimbingUp = false;
		if (getBoard()->isLadder(Point(getX(), getY() + 2)) && !isOnLadder()) {
			this->erase(); 
			setY(getY() + 1);
		}
		isClimbingDown = true;
	}
}

void Mario::handleHorizontal(KEYS keyPressed) {
	if (!isClimbingDown && !isClimbingUp) {
		if (!isOnFloor()) {
			isFalling = true;
		}
		else {
			switch (keyPressed) {
			case KEYS::LEFT:
				setDir(Entity::LEFT);
				break;
			case KEYS::RIGHT:
				setDir(Entity::RIGHT);
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
		setDirY(DOWN);
	}
}


void Mario::fall() {
	isFalling = true;
	setDirY(DOWN);
	if (!isOnFloor()) {
		fallingCounter++;
	}
	else {
		resetFallingCounter = true;
		isFalling = false;
		setDirY(0);
	}
}

void Mario::climb() {
	if (getBoard()->isLadder(Point(getX(), getY() + getDirY()))) {
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

void Mario::resetFallingCounterIfNeeded() {
	if (resetFallingCounter) {
		fallingCounter = 0;
		resetFallingCounter = false;
	}
}