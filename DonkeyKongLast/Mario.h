#pragma once
#include "Entity.h"
#include "GameConfig.h"

class Mario : public Entity {
	
	static constexpr int jumpHeight = 2;
	
	bool isJumping;
	bool isFalling;
	bool isClimbingUp;
	bool isClimbingDown;
	int jumpCounter;
	int fallingCounter;
	bool resetFallingCounter;

public:
	Mario(int x, int y, Board* pBoard) : Entity(ENTITIES_CHARACTERS::MARIO, x, y, pBoard), isJumping(false), isFalling(false), isClimbingUp(false), isClimbingDown(false), jumpCounter(0), fallingCounter(0), resetFallingCounter(false){}
	enum { UP = -1, DOWN = 1, RIGHT = 1, LEFT = -1, STAY = 0 };

	bool isValidKey(KEYS key) {
		return (key == KEYS::UP || key == KEYS::DOWN || key == KEYS::LEFT || key == KEYS::RIGHT || key == KEYS::STAY);
	}
	void keyPressed(KEYS key);
	void move();
	void handleUp();
	void handleDown();
	void handleHorizontal(KEYS keyPressed);
	void jump();
	void climb();
	void fall();
	void resetFallingCounterIfNeeded();
	int getFallingCounter() const{
		return fallingCounter;
	}
	bool isCurrentlyFalling() const {
		return isFalling;
	}
};