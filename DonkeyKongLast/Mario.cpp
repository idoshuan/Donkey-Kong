#include "Mario.h"

// ------------------- Public Functions -------------------

/**
 * @brief Processes the key pressed by the player.
 * Determines the action to take based on the provided key.
 */
void Mario::keyPressed(KEYS keyPressed) {
    if (isValidKey(keyPressed)) {
        switch (keyPressed) {
        case KEYS::UP:
            handleUp();
            return;
        case KEYS::DOWN:
            handleDown();
            return;
        case KEYS::LEFT:
        case KEYS::RIGHT:
            handleHorizontal(keyPressed);
            return;
        case KEYS::STAY:
            setDir(Entity::NONE);
            return;
        default:
            return;
        }
    }
}

/**
 * @brief Updates Mario's movement based on his current state.
 * Handles climbing, jumping, or falling as needed.
 */
void Mario::move() {
    if (isClimbingUp || isClimbingDown) {
        climb();
    }
    else if (isJumping) {
        jump();
    }
    else if (isFalling || !isOnFloor()) {
        fall();
    }
    Entity::move();
}

// ------------------- Private Functions -------------------

/**
 * @brief Handles the logic when Mario moves upward.
 * Initiates climbing if on a ladder or starts a jump.
 */
void Mario::handleUp() {
    if (!isJumping && !isFalling) {
        setDirY(Y_UP);
        if (isOnLadder()) {
            setDirX(X_NONE);
            isClimbingUp = true;
        }
        else {
            isJumping = true;
        }
    }
}

/**
 * @brief Handles the logic when Mario moves downward.
 * Allows climbing down ladders or simply descending.
 */
void Mario::handleDown() {
    if (!isJumping && !isFalling) {
        setDir(Entity::DOWN);
        isClimbingUp = false;

        if (getBoard()->isLadder(Point(getX(), getY() + 2)) && !isOnLadder()) {
            this->erase();
            setY(getY() + 1);
        }
        isClimbingDown = true;
    }
}

/**
 * @brief Handles the logic when Mario moves horizontally.
 * Moves left or right, or initiates falling if no floor is beneath Mario.
 */
void Mario::handleHorizontal(KEYS keyPressed) {
    if (!isClimbingDown && !isClimbingUp) {
        if (!isOnFloor()) {
            isFalling = true;
        }
        else {
            switch (keyPressed) {
            case KEYS::LEFT:
                setDir(Entity::LEFT);
                break;
            case KEYS::RIGHT:
                setDir(Entity::RIGHT);
                break;
            default:
                return;
            }
        }
    }
}

/**
 * @brief Handles Mario's jumping logic.
 * Increases the jump height until the limit is reached, then transitions to falling.
 */
void Mario::jump() {
    if (jumpCounter < jumpHeight && isNextPositionValid()) {
        jumpCounter++;
    }
    else {
        jumpCounter = 0;
        isJumping = false;
        isFalling = true;
        setDirY(Y_DOWN);
    }
}

/**
 * @brief Handles Mario's falling logic.
 * Increases the falling counter and stops falling when a floor is detected.
 */
void Mario::fall() {
    isFalling = true;
    setDirY(Y_DOWN);
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
 * @brief Handles Mario's climbing logic.
 * Moves Mario upward or downward on ladders, stopping when the ladder ends.
 */
void Mario::climb() {
    if (getBoard()->isLadder(Point(getX(), getY() + getDirY()))) {
        return;
    }
    else {
        if (isClimbingUp) {
            setY(getY() - 2);
            isClimbingUp = false;
        }
        else {
            isClimbingDown = false;
        }
        setDirY(Y_NONE);
    }
}

/**
 * @brief Resets Mario's falling counter if a floor is detected.
 * Prevents the falling counter from continuing unnecessarily.
 */
void Mario::resetFallingCounterIfNeeded() {
    if (resetFallingCounter) {
        fallingCounter = 0;
        resetFallingCounter = false;
    }
}

// ------------------- Utility Functions -------------------

/**
 * @brief Checks if the given key is valid for Mario's movement.
 * @param key The key to validate.
 * @return True if the key is valid; false otherwise.
 */
bool Mario::isValidKey(KEYS key) const {
    return (key == KEYS::UP || key == KEYS::DOWN || key == KEYS::LEFT || key == KEYS::RIGHT || key == KEYS::STAY);
}

/**
 * @brief Gets the current value of Mario's falling counter.
 * @return The falling counter value.
 */
int Mario::getFallingCounter() const {
    return fallingCounter;
}

/**
 * @brief Checks if Mario is currently falling.
 * @return True if Mario is falling; false otherwise.
 */
bool Mario::isCurrentlyFalling() const {
    return isFalling;
}
