#pragma once
#include "Point.h"

class Barrel: public Point{
	static constexpr char character = 'O';

	int fallingCounter;
	bool isFalling;
	bool resetFallingCounter;
	bool isActive;
	bool isExplode;
public:
	Barrel(int x = 0, int y = 0, Board* pBoard = nullptr) : Point(character, x, y, pBoard), fallingCounter(0), isFalling(false), resetFallingCounter(false), isActive(false), isExplode(false){}
	
	void move();
	void fall();
	void resetFallingCounterIfNeeded();
	bool didExplode() {
		return isExplode;
	}
	void explode() {
		isExplode = true;
	}
	bool isCurrentlyActive() {
		return isActive;
	}
	void deactivate() {
		isActive = false;
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

