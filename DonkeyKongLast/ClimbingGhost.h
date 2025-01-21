#pragma once
#include "GhostBase.h"

class ClimbingGhost : public GhostBase {
	// ------------------- State Variables -------------------
	
	bool isClimbingUp = false;
	bool isClimbingDown = false;

	
public:
	ClimbingGhost(Point pos, Board& board) : GhostBase(ENTITIES_CHARACTERS::CLIMBING_GHOST, pos, board) {
		setDirX(getRandomDouble() >= 0.5 ? Entity::X_LEFT : Entity::X_RIGHT);
	}

	// ------------------- Action Functions -------------------
	void move() override;
	
};