#pragma once
#include "Point.h"

class Mario : public Point {
	
	static constexpr char keys[] = { 'w', 'x', 'a', 'd', 's'};
	static constexpr size_t numKeys = sizeof(keys) / sizeof(char);
	static constexpr int jumpHeight = 2;

	bool isJumping;
	bool isFalling;
	bool isClimbingUp;
	bool isClimbingDown;
	int jumpCounter;
	int fallingCounter;

public:
	Mario(Board* pBoard) : Point('@', 4, 23, pBoard), isJumping(false), isFalling(false), isClimbingUp(false), isClimbingDown(false), jumpCounter(0), fallingCounter(0) {}

	void keyPressed(char key);
	void move();
	void handleUp();
	void handleDown();
	void jump();
	void climb();
	void fall();
};