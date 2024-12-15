#include "Barrel.h"

/**
 * @brief Handles the barrel's movement on the board.
 * The barrel changes direction based on the type of floor ('<', '>', or '=').
 * If the barrel is not on a floor or already falling, it initiates falling.
 */
void Barrel::move() {
    if (floorDir() == '<') {
        setDirX(X_LEFT);       
        prevDirX = X_LEFT;
    }
    else if (floorDir() == '>') {
        setDirX(X_RIGHT);      
        prevDirX = X_RIGHT;
    }
    else if (floorDir() == '=') {
        setDirX(prevDirX);     
    }

    if (!isOnFloor() || isFalling) {
        fall();                
    }

    Entity::move();            
}

/**
 * @brief Handles the falling logic for the barrel.
 * Updates the barrel's falling state and increments the falling counter
 * until the barrel lands on a floor.
 */
void Barrel::fall() {
    isFalling = true;
    setDir(Entity::DOWN);      

    if (!isOnFloor()) {
        fallingCounter++;      
    }
    else {
        resetFallingCounter = true;  
        isFalling = false;          
        setDirY(Y_NONE);           
    }
}

/**
 * @brief Resets the falling counter if the barrel has landed on a floor.
 * This ensures the counter does not accumulate unnecessarily.
 */
void Barrel::resetFallingCounterIfNeeded() {
    if (resetFallingCounter) {
        fallingCounter = 0;        
        resetFallingCounter = false;
    }
}
