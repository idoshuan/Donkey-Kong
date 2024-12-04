#pragma once
#include "Board.h"

class Point {
	struct Direction {
		int x, y;

		bool operator==(const Direction& other) const {
			return x == other.x && y == other.y;
		}
	};

	static constexpr Direction UP = { 0, -1 };
	static constexpr Direction DOWN = { 0, 1 };
	static constexpr Direction LEFT = { -1, 0 };
	static constexpr Direction RIGHT = { 1, 0 };
	static constexpr Direction NONE = { 0, 0 };
	static constexpr Direction directions[] = { UP, DOWN, LEFT, RIGHT, NONE};

	char ch;
	int x, y;
	Board* pBoard;
	Direction dir;

	void drawCharacter(char c) const {
		gotoxy(x, y);
		std::cout << c;
	}

public:
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
		return pBoard->isFloorBellow(x, y);
	}
	bool isOnLadder() const {
		return pBoard->getChar(x, y) == 'H';
	}
	bool isNextPositionValid() const {
		return pBoard->isValidPosition(x + dir.x, y + dir.y);
	}
	void setDir(Direction newDir) {
		dir = newDir;
	}
	const Direction& getDir(size_t index) {
		if (index < sizeof(directions) / sizeof(Direction)) {
			return directions[index];
		}
		else {
			return NONE;
		}
	}
	void stop() {
		dir = NONE;
	}
};


