#pragma once
#include "Utils.h"
#include "Point.h"
#include "GameConfig.h"
#include "Screen.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#pragma warning(disable : 26495) // Disable warning for uninitialized variables


/**
 * @class Board
 * @brief Represents the game board and handles its operations such as resetting, printing, and position validations.
 * Note: This entire class is based on the board class showed to us in class
 */


// ------------------- Class Declaration -------------------
class Board : public Screen {
private:
    // ------------------- Constants -------------------
    Point paulinaPos, marioPos, donkeyPos,hammerPos, legendPos;
    std::vector<Point> ghostsPos;

    int minX = SCREEN_BOUNDARIES::MIN_X, maxX = SCREEN_BOUNDARIES::MAX_X;
    int minY = SCREEN_BOUNDARIES::MIN_Y, maxY = SCREEN_BOUNDARIES::MAX_Y;

    char originalBoard[SCREEN_BOUNDARIES::MAX_Y][SCREEN_BOUNDARIES::MAX_X + 1]; // +1 for null terminator
    char currentBoard[SCREEN_BOUNDARIES::MAX_Y][SCREEN_BOUNDARIES::MAX_X + 1]; // +1 for null terminator

    bool isFloorBelowInFile(Point p) {
        char ch = originalBoard[p.getY()+1][p.getX()];
        return ch == BOARD_CHARACTERS::FLOOR || ch == BOARD_CHARACTERS::LEFT_FLOOR || ch == BOARD_CHARACTERS::RIGHT_FLOOR || ch == BOARD_CHARACTERS::QFLOOR;
    }

public:
    /**
     * @brief Initializes the game board by resetting it and setting the locations of Paulina and Donkey Kong.
     * The `currentBoard` is updated to place Donkey Kong and Paulina at their respective positions,
     * as defined in the `CHARACTER_POSITIONS` structure.
     */
    void reset();
    bool load(const std::string& filename, std::string* errors);
    void print() const;
    bool isValidPosition(const Point p) const;

    char getChar(const Point p) const {
        return originalBoard[p.getY()][p.getX()];
    }
    bool isLadder(const Point p) const {
        return getChar(p) == BOARD_CHARACTERS::LADDER;
    }
    bool isFloorBelow(const Point p) const {
        char ch = getChar(Point(p.getX(), p.getY() + 1));
        return ch == BOARD_CHARACTERS::FLOOR || ch == BOARD_CHARACTERS::LEFT_FLOOR || ch == BOARD_CHARACTERS::RIGHT_FLOOR || ch == BOARD_CHARACTERS::QFLOOR;
    }
    int getMaxX() const{
        return maxX;
    }
    int getMinX() const{
        return minX;
    }

    Point getPaulinaPos() const{
        return paulinaPos;
    }
    Point getDonkeyKongPos() const{
        return donkeyPos;
    }
    Point getMarioPos() const {
        return marioPos;
    }
    Point getHammerPos() const {
        return hammerPos;
    }
    Point getLegendPos() const {
        return legendPos;
    }
    std::vector<Point> getGhostsPos() const {
        return ghostsPos;
    }
    void deleteHammer() {
        gotoxy(hammerPos.getX(), hammerPos.getY());
        std::cout << static_cast<char>(BOARD_CHARACTERS::AIR);
        originalBoard[hammerPos.getY()][hammerPos.getX()] = static_cast<char>(BOARD_CHARACTERS::AIR);
    }
    void reviveHammer() {
        gotoxy(hammerPos.getX(), hammerPos.getY());
        std::cout << static_cast<char>(BOARD_CHARACTERS::HAMMER);
        originalBoard[hammerPos.getY()][hammerPos.getX()] = static_cast<char>(BOARD_CHARACTERS::HAMMER);
    }
};
