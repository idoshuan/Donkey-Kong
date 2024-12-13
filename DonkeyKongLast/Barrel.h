#pragma once
#include "Entity.h"

class Barrel: public Entity{

	int fallingCounter = 0;
	bool isFalling = false;
	bool resetFallingCounter = false;
	bool isActive = false; 
	bool isExplode = false;
	int prevDirX = 0;

public:
	Barrel(Point initPoint = Point{ 0, 0 }, Board* pBoard = nullptr) : Entity(ENTITIES_CHARACTERS::BARREL, initPoint, pBoard) {}
	
	void move();
	void fall();
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

