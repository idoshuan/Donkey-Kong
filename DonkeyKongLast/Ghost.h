#pragma once
#include "GhostBase.h"

#include <iostream>


// ------------------- Class Declaration -------------------
class Ghost : public GhostBase {
	

public:
	// ------------------- Constructors -------------------
	Ghost(Point pos, Board& board) : GhostBase(ENTITIES_CHARACTERS::GHOST, pos, board) {}

	// ------------------- Action Functions -------------------
	void move() override;
	void collision() override{
		turnAround();
		inCollision = true;
	}
};