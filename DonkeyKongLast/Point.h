#pragma once
#include "Board.h"

class Point {
	struct Direction {
		int x, y;

		bool operator==(const Direction& other) const {
			return x == other.x && y == other.y;
		}
	};

	char ch;
	int x, y;
	Board* pBoard;
	Direction dir;

	void drawCharacter(char c) const {
		gotoxy(x, y);
		std::cout << c;
	}

public:
	static constexpr Direction UP = { 0, -1 };
	static constexpr Direction DOWN = { 0, 1 };
	static constexpr Direction LEFT = { -1, 0 };
	static constexpr Direction RIGHT = { 1, 0 };
	static constexpr Direction NONE = { 0, 0 };
	static constexpr Direction directions[] = { UP, DOWN, LEFT, RIGHT, NONE };
	static constexpr int up = -1;
	static constexpr int down = 1;

	Point(char ch, int x, int y, Board* pBoard = nullptr, Direction dir = NONE) : ch(ch), x(x), y(y), dir(dir), pBoard(pBoard) {}

	void move();
	void setBoard(Board& board) {
		pBoard = &board;
	}
	void draw() const {
		drawCharacter(ch);
	}
	void erase() {
		char backgroundChar = pBoard->getChar(x, y);
		drawCharacter(backgroundChar);
	}
	bool isOnFloor() const {
		return pBoard->isFloorBelow(x, y);
	}
	bool isOnLadder() const {
		return pBoard->getChar(x, y) == 'H';
	}
	bool isLadder(int x, int y) const {
		return pBoard->getChar(x, y) == 'H';
	}
	bool isNextPositionValid() const {
		return pBoard->isValidPosition(x + dir.x, y + dir.y);
	}
	Direction getDir() const{
		return dir;
	}
	int getX() const{
		return x;
	}
	int getY() const{
		return y;
	}
	void setX(int newX) {
		x = newX;
	}
	void setY(int newY) {
		y = newY;
	}
	int getDirX() const{
		return dir.x;
	}
	int getDirY() const{
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
	const Direction& findDir(size_t index) {
		if (index < sizeof(directions) / sizeof(Direction)) {
			return directions[index];
		}
		else {
			return NONE;
		}
	}
	void stop() {
		dir.x = 0;
	}
};


