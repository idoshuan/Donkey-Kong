#pragma once
#include "Entity.h"
#include "GameConfig.h"

class Mario : public Entity {
	
	static constexpr int jumpHeight = 2;
	
	bool isJumping = false;
	bool isFalling = false;
	bool isClimbingUp = false;
	bool isClimbingDown = false;
	int jumpCounter = 0;
	int fallingCounter = 0;
	bool resetFallingCounter = false;

public:
	Mario(Point initPoint, Board* pBoard) : Entity(ENTITIES_CHARACTERS::MARIO, initPoint, pBoard){}

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