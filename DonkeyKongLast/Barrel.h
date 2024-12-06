#pragma once
#include "Point.h"

class Barrel: public Point{
	int fallingCounter;
	bool isFalling;

public:
	Barrel(int x, int y, Board* pBoard) : Point('O', x, y, pBoard), fallingCounter(0), isFalling(false) {}
	
	void move();
	void fall();
};

