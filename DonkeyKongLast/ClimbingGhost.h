#pragma once
#include "Entity.h"

class ClimbingGhost : public Entity {
	// ------------------- State Variables -------------------
	bool isActive = true;
	bool inCollision = false;
	bool isClimbingUp = false;
	bool isClimbingDown = false;

	// ------------------- Utility Functions -------------------
	void setRandDir();
public:
	ClimbingGhost() : Entity(ENTITIES_CHARACTERS::CLIMBING_GHOST) {}
	ClimbingGhost(Point pos, Board& board) : Entity(ENTITIES_CHARACTERS::CLIMBING_GHOST, pos, &board) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
	}

	// ------------------- Action Functions -------------------
	void move();
	void deactivate() {
		isActive = false;
	}
	void activate() {
		isActive = true;
	}
	void collision() {
		turnAround();
		inCollision = true;
	}

	// ------------------- State Check Functions -------------------
	bool isCurrentlyActive() const {
		return isActive;
	}
};