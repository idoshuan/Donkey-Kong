#pragma once
#include "Board.h"
#include "Point.h"
#include "GameConfig.h"
#include <iostream>

class Entity {
private:
	struct Direction {
		int x, y;
	};

	ENTITIES_CHARACTERS ch;
	Point point;
	Board* pBoard;
	Direction dir;

	void drawCharacter(char c) const {
		gotoxy(getX(), getY());
		std::cout << c;
	}

public:
	static constexpr Direction DOWN = { 0, 1 };
	static constexpr Direction LEFT = { -1, 0 };
	static constexpr Direction RIGHT = { 1, 0 };
	static constexpr Direction NONE = { 0, 0 };
	static enum { Y_UP = -1, Y_DOWN = 1, X_RIGHT = 1, X_LEFT = -1, X_NONE = 0, Y_NONE = 0 };

	Entity(ENTITIES_CHARACTERS ch, Point initPoint, Board* pBoard = nullptr, Direction dir = NONE) : ch(ch), point(initPoint), pBoard(pBoard), dir(dir) {}

	int getX() const {
		return point.getX();
	}
	int getY() const {
		return point.getY();
	}

	void setX(int newX) {
		point.setX(newX);
	}
	void setY(int newY) {
		point.setY(newY);
	}

	Direction getDir() const {
		return dir;
	}
	int getDirX() const {
		return dir.x;
	}
	int getDirY() const {
		return dir.y;
	}

	void setDir(Direction newDir) {
		dir = newDir;
	}
	void setDirX(int newX) {
		dir.x = newX;
	}
	void setDirY(int newY) {
		dir.y = newY;
	}

	void setBoard(Board& board) {
		pBoard = &board;
	}
	Board* getBoard() {
		return pBoard;
	}

	bool isOnFloor() const {
		return pBoard->isFloorBelow(point);
	}
	bool isOnLadder() const {
		return pBoard->getChar(point) == BOARD_CHARACTERS::LADDER;
	}

	bool isNextPositionValid() const {
		return pBoard->isValidPosition(Point(getX() + dir.x, getY() + dir.y));
	}

	void draw() const {
		drawCharacter(static_cast<char>(ch));
	}
	void erase() {
		char backgroundChar = pBoard->getChar(Point(getX(), getY()));
		drawCharacter(backgroundChar);
	}

	void move();
	char floorDir() {
		return pBoard->getChar(Point(getX(), getY() + 1));
	}

	void stop() {
		dir = NONE;
	}
};
