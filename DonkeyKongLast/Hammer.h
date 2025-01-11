#pragma once
#include "Mario.h"

class Hammer
{
	Mario& mario;
	Point pos = { mario.getX() + Entity::X_RIGHT,mario.getY() };
	char ch = 'p';

	// ------------------- Utility Functions -------------------
	void drawCharacter(char c) const {
		gotoxy(pos.getX(), pos.getY());
		std::cout << c;
	}

public:
	Hammer(Mario& mario) : mario(mario) {}

	void move() {
		if (mario.getDirX() != Entity::X_NONE) {
			pos.setX(mario.getX() + mario.getDirX());
		}
		pos.setY(mario.getY());
	}
	void draw() const {
		drawCharacter(static_cast<char>(ch));
	}
	void erase() {
		char backgroundChar = mario.getBoard()->getChar(Point(pos.getX(), pos.getY()));
		drawCharacter(backgroundChar);
	}

};

