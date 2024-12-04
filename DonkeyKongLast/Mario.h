#pragma once
#include "Point.h"

class Mario : public Point {
	
	static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's' };
	static constexpr size_t numKeys = sizeof(keys) / sizeof(char);
	static constexpr int jumpHeight = 2;

	bool isJumping;
	bool isFalling;
	bool isClimbing;

public:
	Mario(Board* pBoard) : Point('@', 4, 23), isJumping(false), isFalling(false), isClimbing(false) {}	

	void keyPressed(char key);
};