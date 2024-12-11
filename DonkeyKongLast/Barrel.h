#pragma once
#include "Entity.h"

class Barrel: public Entity{
	static constexpr char character = 'O';

	int fallingCounter;
	bool isFalling;
	bool resetFallingCounter;
	bool isActive;
	bool isExplode;
	int prevDirX;

public:
	Barrel(int x = 0, int y = 0, Board* pBoard = nullptr) : Entity(character, x, y, pBoard), fallingCounter(0), isFalling(false), resetFallingCounter(false), isActive(false), isExplode(false), prevDirX(0){}
	
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

