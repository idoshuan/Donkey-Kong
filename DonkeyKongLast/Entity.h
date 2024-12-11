#pragma once
#include "Board.h"
#include "Point.h"

class Entity {
	struct Direction {
		int x, y;

		bool operator==(const Direction& other) const {
			return x == other.x && y == other.y;
		}
	};

	char ch;
	Point point;
	Board* pBoard;
	Direction dir;

	void drawCharacter(char c) const {
		gotoxy(getX(), getY());
		std::cout << c;
	}

public:
	static constexpr Direction UP = { 0, -1 };
	static constexpr Direction DOWN = { 0, 1 };
	static constexpr Direction LEFT = { -1, 0 };
	static constexpr Direction RIGHT = { 1, 0 };
	static constexpr Direction NONE = { 0, 0 };
	static constexpr int up = -1;
	static constexpr int down = 1;
	static constexpr int left = -1;
	static constexpr int right = 1;

	Entity(char ch, int x, int y, Board* pBoard = nullptr, Direction dir = NONE) : ch(ch), point(x, y), pBoard(pBoard), dir(dir) {}

	void move();
	int getX() const{
		return point.getX();
	}
	int getY() const{
		return point.getY();
	}
	void setBoard(Board& board) {
		pBoard = &board;
	}
	Board* getBoard() {
		return pBoard;
	}
	void draw() const {
		drawCharacter(ch);
	}
	void erase() {
		char backgroundChar = pBoard->getChar(getX(), getY());
		drawCharacter(backgroundChar);
	}
	bool isOnFloor() const {
		return pBoard->isFloorBelow(getX(), getY());
	}
	bool isOnLadder() const {
		return pBoard->getChar(getX(), getY()) == 'H';
	}
	bool isNextPositionValid() const {
		return pBoard->isValidPosition(getX() + dir.x, getY() + dir.y);
	}
	void setX(int newX) {
		point.setX(newX);
	}
	void setY(int newY) {
		point.setY(newY);
	}
	int getDirX() const{
		return dir.x;
	}
	int getDirY() const{
		return dir.y;
	}
	Direction getDir() const {
		return dir;
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
	char floorDir() {
		return pBoard->getChar(getX(), getY() + 1);
	}
	void stop() {
		dir.x = 0;
	}
};


