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

    void fillMissingColumns(int currCol, int currRow);
    void fillBoardBoundaries(int lastRow);
    void handleUnknownCharacter(char c, int currCol, int currRow, bool& isValid, std::string& errorMessages);
    void handleMario(char& c, int currCol, int currRow, bool& hasMario, Point& marioPos, bool& isValid, std::string& errorMessages);
    void handlePaulina(char& c, int currCol, int currRow, bool& hasPaulina, Point& paulinaPos, bool& isValid, std::string& errorMessages);
    void handleDonkey(char& c, int currCol, int currRow, bool& hasDonkey, Point& donkeyPos, bool& isValid, std::string& errorMessages);
    void handleGhost(char& c, int currCol, int currRow, std::vector<Point>& ghostsPos);
    void handleHammer(char& c, int currCol, int currRow, bool& hasHammer, Point& hammerPos, bool& isValid);
    void handleLegend(char& c, int currCol, int currRow, bool& hasLegend, Point& legendPos, bool& isValid, std::string& errorMessages);
    void handleBorderCharacter(char& c, int currCol, int currRow, bool& isBounded, bool& isValid, std::string& errorMessages);


public:
    /**
     * @brief Initializes the game board by resetting it and setting the locations of Paulina and Donkey Kong.
     * The `currentBoard` is updated to place Donkey Kong and Paulina at their respective positions,
     * as defined in the `CHARACTER_POSITIONS` structure.
     */
    void reset();
    void print() const;
    bool isValidPosition(const Point p) const;
    bool load(const std::string& filename, std::string* errors);

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
    const std::vector<Point>& getGhostsPos() const {
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
