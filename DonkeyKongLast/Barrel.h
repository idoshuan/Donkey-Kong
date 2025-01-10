#pragma once
#include "Entity.h"

/**
 * @brief The Barrel class represents barrels in the game, inheriting from Entity.
 * It handles barrel-specific mechanics such as movement, falling, activation,
 * and explosion. Barrels interact with the board and other entities like Mario.
 */


// ------------------- Class Declaration -------------------
class Barrel : public Entity {
private:
	// ------------------- State Variables -------------------
	int fallingCounter = 0;
	bool isFalling = false;
	bool resetFallingCounter = false;
	bool isActive = false;
	bool isExplode = false;
	int prevDirX = 0;

	// ------------------- Private Functions -------------------
	void fall();

public:
	// ------------------- Constructor -------------------
	Barrel(): Entity(ENTITIES_CHARACTERS::BARREL) {}
	Barrel(Point pos, Board& board) : Entity(ENTITIES_CHARACTERS::BARREL, pos, &board) {}

	// ------------------- Public Functions -------------------
	void move();
	void resetFallingCounterIfNeeded();
	void explode() {
		isExplode = true;
	}
	bool didExplode() const{
		return isExplode;
	}
	void deactivate() {
		isActive = false;
	}
	bool isCurrentlyActive() const{
		return isActive;
	}
	void activate() {
		isActive = true;
	}
	int getFallingCounter() const{ 
		return fallingCounter; 
	}
	bool isCurrentlyFalling() const {
		return isFalling;
	}
};

