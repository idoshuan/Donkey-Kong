#pragma once
#include "GhostBase.h"

#include <iostream>


// ------------------- Class Declaration -------------------
class Ghost : public GhostBase {
	

public:
	// ------------------- Constructors -------------------
	Ghost(Point pos, Board& board) : GhostBase(ENTITIES_CHARACTERS::GHOST, pos, board) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
	}

	// ------------------- Action Functions -------------------
	void move() override;
	
};