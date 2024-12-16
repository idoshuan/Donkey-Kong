#pragma once
#include "Board.h"
#include "Point.h"
#include "GameConfig.h"
#include <iostream>

/**
 * @brief The Entity class serves as a base class for all moving entities in the game.
 * It manages position, direction, movement, and interactions with the game board.
 * Note: This entire class is based on the Point class showed to us in class.
 */

 // ------------------- Class Declaration -------------------
class Entity {
private:
	/**
	 * @brief Represents a direction vector with x and y components.
	 */
	struct Direction {
		int x, y;
	};

	// ------------------- Data Members -------------------
	ENTITIES_CHARACTERS ch; // Character representing the entity
	Point point;            // Entity's position
	Board* pBoard;          // Pointer to the game board
	Direction dir;          // Direction of movement

	// ------------------- Utility Functions -------------------
	void drawCharacter(char c) const {
		gotoxy(getX(), getY());
		std::cout << c;
	}

public:
	// ------------------- Constructor -------------------
	Entity(ENTITIES_CHARACTERS ch, Point initPoint, Board* pBoard = nullptr, Direction dir = NONE) : ch(ch), point(initPoint), pBoard(pBoard), dir(dir){};

	// ------------------- Constants -------------------
	static constexpr Direction DOWN = { 0, 1 };
	static constexpr Direction LEFT = { -1, 0 };
	static constexpr Direction RIGHT = { 1, 0 };
	static constexpr Direction NONE = { 0, 0 };

	static enum {
		Y_UP = -1, Y_DOWN = 1,
		X_RIGHT = 1, X_LEFT = -1,
		X_NONE = 0, Y_NONE = 0
	};

	// ------------------- Getters -------------------
	int getX() const {
		return point.getX();
	}
	int getY() const {
		return point.getY();
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
	Board* getBoard() {
		return pBoard;
	}

	// ------------------- Getters -------------------
	void setX(int newX) {
		point.setX(newX);
	}
	void setY(int newY) {
		point.setY(newY);
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

	// ------------------- State Check Functions -------------------
	bool isOnFloor() const {
		return pBoard->isFloorBelow(point);
	}
	bool isOnLadder() const {
		return pBoard->getChar(point) == BOARD_CHARACTERS::LADDER;
	}

	bool isNextPositionValid() const {
		return pBoard->isValidPosition(Point(getX() + dir.x, getY() + dir.y));
	}

	// ------------------- Action Functions -------------------
	void move();
	void draw() const {
		drawCharacter(static_cast<char>(ch));
	}
	void erase() {
		char backgroundChar = pBoard->getChar(Point(getX(), getY()));
		drawCharacter(backgroundChar);
	}
	char floorDir() {
		return pBoard->getChar(Point(getX(), getY() + 1));
	}
	void stop() {
		dir = NONE;
	}
};
