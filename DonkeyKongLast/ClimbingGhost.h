#pragma once
#include "GhostBase.h"
#include "Point.h"

class ClimbingGhost : public GhostBase {
	// ------------------- State Variables -------------------
	
	bool isClimbingUp = false;
	bool isClimbingDown = false;

	
public:
	// ------------------- Constructor -------------------
	ClimbingGhost(Point pos, Board& board) : GhostBase(ENTITIES_CHARACTERS::CLIMBING_GHOST, pos, board) {}

	// ------------------- Action Functions -------------------
	void move() override;
	void collision() override;
	bool shouldTurnAround();
	bool canStartClimbingUp();
	void startClimbingUp();
	void stopClimbingUp();
	bool shouldStartClimbingDown();
	void startClimbingDown();
	bool finishedClimbing();
	void changeDirectionRandomly();
};