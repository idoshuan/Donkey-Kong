#pragma once
#include "Entity.h"
#include "Utils.h"

#include <iostream>


// ------------------- Class Declaration -------------------
class GhostBase : public Entity {

	// ------------------- State Variables -------------------
	static constexpr double turnProbability = 0.05;
	bool isActive = true;

protected:

	bool inCollision = false;
	// ------------------- Utility Functions -------------------
	void setRandDir();

public:
	// ------------------- Constructors -------------------
	GhostBase(ENTITIES_CHARACTERS ch,Point pos, Board& board) : Entity(ch, pos, &board) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
	}


	// ------------------- Action Functions -------------------
	virtual void move() = 0;
	virtual void collision() = 0;

	void deactivate() {
		isActive = false;
	}
	void activate() {
		isActive = true;
	}

	
	// ------------------- State Check Functions -------------------
	bool isCurrentlyActive() const {
		return isActive;
	}
};