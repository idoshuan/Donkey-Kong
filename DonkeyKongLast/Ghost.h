#pragma once
#include "Entity.h"
#include "Utils.h"

#include <iostream>


class Ghost : public Entity {
	bool isActive = true;
	bool inCollision = false;

	void setRandDir();

public:
	Ghost() : Entity(ENTITIES_CHARACTERS::GHOST) {}
	Ghost(Point pos, Board& board) : Entity(ENTITIES_CHARACTERS::GHOST, pos, &board) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
	}
	void move();
	void deactivate() {
		isActive = false;
	}
	bool isCurrentlyActive() const {
		return isActive;
	}
	void activate() {
		isActive = true;
	}
	void collision() {
		turnAround();
		inCollision = true;
	}
};