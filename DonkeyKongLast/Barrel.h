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
	Barrel(Point initPoint = Point{ 0, 0 }, Board* pBoard = nullptr)
		: Entity(ENTITIES_CHARACTERS::BARREL, initPoint, pBoard) {
	}

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

