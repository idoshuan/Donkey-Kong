#pragma once
#include "Point.h"

class Mario : public Point {
	
	static constexpr char keys[] = { 'w', 'x', 'a', 'd', 's'};
	static constexpr size_t numKeys = sizeof(keys) / sizeof(char);
	static constexpr char character = '@';
	static constexpr int jumpHeight = 2;
	

	bool isJumping;
	bool isFalling;
	bool isClimbingUp;
	bool isClimbingDown;
	int jumpCounter;
	int fallingCounter;
	int lives;

public:
	Mario(int x, int y, Board* pBoard) : Point(character, x, y, pBoard), isJumping(false), isFalling(false), isClimbingUp(false), isClimbingDown(false), jumpCounter(0), fallingCounter(0), lives(3) {}

	bool isValidKey(char k) {
		char key = std::tolower(k);
		return (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'x');
	}
	void keyPressed(char key);
	void move();
	void handleUp();
	void handleDown();
	void handleHorizontal(char keyPressed);
	void jump();
	void climb();
	void fall();
};