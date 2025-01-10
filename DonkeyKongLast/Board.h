#pragma once
#include "Utils.h"
#include "Point.h"
#include "GameConfig.h"
#include "Screen.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>


/**
 * @class Board
 * @brief Represents the game board and handles its operations such as resetting, printing, and position validations.
 * Note: This entire class is based on the board class showed to us in class
 */


// ------------------- Class Declaration -------------------
class Board : public Screen {
private:
    // ------------------- Constants -------------------
    Point paulina, mario, donkey;
    std::vector<Point> ghosts;

    char originalBoard[SCREEN_BOUNDARIES::MAX_Y][SCREEN_BOUNDARIES::MAX_X + 1]; // +1 for null terminator
    char currentBoard[SCREEN_BOUNDARIES::MAX_Y][SCREEN_BOUNDARIES::MAX_X + 1]; // +1 for null terminator

public:
    /**
     * @brief Initializes the game board by resetting it and setting the locations of Paulina and Donkey Kong.
     * The `currentBoard` is updated to place Donkey Kong and Paulina at their respective positions,
     * as defined in the `CHARACTER_POSITIONS` structure.
     */
    void reset();
    void load(const std::string& filename);
    void print() const;
    bool isValidPosition(const Point p) const;

    char getChar(const Point p) const {
        return currentBoard[p.getY()][p.getX()];
    }
    bool isLadder(const Point p) const {
        return getChar(p) == BOARD_CHARACTERS::LADDER;
    }
    bool isFloorBelow(const Point p) const {
        char ch = getChar(Point(p.getX(), p.getY() + 1));
        return ch == BOARD_CHARACTERS::FLOOR || ch == BOARD_CHARACTERS::LEFT_FLOOR || ch == BOARD_CHARACTERS::RIGHT_FLOOR;
    }
    int getMaxX() const{
        return SCREEN_BOUNDARIES::MAX_X;
    }
    int getMinX() const{
        return SCREEN_BOUNDARIES::MIN_X;
    }

    Point getPaulina() const{
        return paulina;
    }
    Point getDonkeyKong() const{
        return donkey;
    }
    Point getMario() const {
        return mario;
    }
    std::vector<Point> getGhosts() const {
        return ghosts;
    }
};
