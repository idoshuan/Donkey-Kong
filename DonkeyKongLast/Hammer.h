#pragma once
#include "Mario.h"
#include "GameConfig.h"

/**
 * @brief The Hammer class represents the hammer in the game, associated with Mario.
 * It handles hammer-specific mechanics such as swinging, movement with Mario,
 * and interactions with the board. The hammer can swing to deactivate enemies
 * like barrels and ghosts.
 */


 // ------------------- Class Declaration -------------------
class Hammer {
    // ------------------- Data Members -------------------
    Mario& mario;
    Point pos = { mario.getX() + Entity::X_RIGHT, mario.getY() };
    char ch = static_cast<char>(BOARD_CHARACTERS::HAMMER);
    bool isSwinging = false;

    // ------------------- Utility Functions -------------------
    void drawCharacter(char c) const {
        gotoxy(pos.getX(), pos.getY());
        std::cout << c;
    }

public:
    // ------------------- Constructor -------------------
    Hammer(Mario& mario) : mario(mario) {}

    // ------------------- Action Functions -------------------
    void move();
    void draw() const {
        drawCharacter(static_cast<char>(ch));
    }
    void erase() {
        char backgroundChar = mario.getBoard()->getChar(Point(pos.getX(), pos.getY()));
        drawCharacter(backgroundChar);
    }
    void swing() {
        isSwinging = true;
    }
    void stopSwing() {
        isSwinging = false;
    }

    // ------------------- State Check Functions -------------------
    bool isCurrentlySwinging() {
        return isSwinging;
    }

    // ------------------- Getters -------------------
    Point getPos() {
        return pos;
    }
    int getX() {
        return pos.getX();
    }
    int getY() {
        return pos.getY();
    }
};
