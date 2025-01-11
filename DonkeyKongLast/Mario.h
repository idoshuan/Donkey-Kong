#pragma once
#include "Entity.h"
#include "GameConfig.h"

/**
 * @brief The Mario class represents the player's character in the game,
 * inheriting from Entity. It manages Mario's movement, jumping, climbing,
 * and falling mechanics, and processes player inputs to control his actions.
 */

// ------------------- Class Declaration -------------------
class Mario : public Entity {
private:
    // ------------------- Constants -------------------
    static constexpr int jumpHeight = 2;
    static constexpr int marioMaxFallHeight = 5;

    // ------------------- State Variables -------------------
    bool isJumping = false;
    bool isFalling = false;
    bool isClimbingUp = false;
    bool isClimbingDown = false;
    int jumpCounter = 0;
    int fallingCounter = 0;
    bool resetFallingCounter = false;
    
    void handleUp();
    void handleDown();
    void handleHorizontal(KEYS key);
    void jump();
    void climb();
    void fall();
    bool isValidKey(KEYS key) const;

public:
    // ------------------- Constructor -------------------
    Mario() : Entity(ENTITIES_CHARACTERS::MARIO) {}
    Mario(Board& board) : Entity(ENTITIES_CHARACTERS::MARIO, board.getMarioPos(), &board) {}


    // ------------------- Public Functions -------------------
    void keyPressed(KEYS key);
    void move();
    void resetFallingCounterIfNeeded();
    int getFallingCounter() const;
    bool isCurrentlyFalling() const;
};
