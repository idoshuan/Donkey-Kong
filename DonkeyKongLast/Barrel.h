#pragma once
#include "Point.h"

class Barrel: public Point{
	static constexpr char character = 'O';

	int fallingCounter;
	bool isFalling;

public:
	Barrel(int x = 0, int y = 0, Board* pBoard = nullptr) : Point(character, x, y, pBoard), fallingCounter(0), isFalling(false) {}
	
	void move();
	void fall();
};

