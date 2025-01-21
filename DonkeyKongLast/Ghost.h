#pragma once
#include "Entity.h"
#include "Utils.h"

#include <iostream>


// ------------------- Class Declaration -------------------
class Ghost : public Entity {
	
	// ------------------- State Variables -------------------
	static constexpr double turnProbability = 0.05;
	bool isActive = true;
	bool inCollision = false;

	// ------------------- Utility Functions -------------------
	void setRandDir();
public:
	// ------------------- Constructors -------------------
	Ghost() : Entity(ENTITIES_CHARACTERS::GHOST) {}
	Ghost(Point pos, Board& board) : Entity(ENTITIES_CHARACTERS::GHOST, pos, &board) {
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