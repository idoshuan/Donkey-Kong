#pragma once
#include "Utils.h"
#include "Point.h"
#include "GameConfig.h"
#include "Screen.h"
#include <iostream>

/**
 * @class Board
 * @brief Represents the game board and handles its operations such as resetting, printing, and position validations.
 */


// ------------------- Class Declaration -------------------
class Board : public Screen {
private:
    // ------------------- Constants -------------------
    struct CHARACTER_POSITIONS {
        static constexpr Point paulina = { 39, 0 };
        static constexpr Point donkeyKong = { 39, 2 };
    };

    const char* originalBoard[SCREEN_BOUNDARIES::MAX_Y] = {
        //          1         2         3         4         5         6         7
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789 
         "Lives:                                                                          ", //0
         "             =======================================================            ", //1
         "               H                                                 H              ", //2
         "               H        <<<<<<<<<<<<<<<=>>>>>>>>>>>>>>>>         H              ", //3
         "               H                                                 H              ", //4
         "               =========                                ==========              ", //5
         "                      H                                  H                      ", //6
         "                      H                                  H                      ", //7
         "     =======>>>============                      ================<<<=======     ", //8
         "             H           H                         H                            ", //9
         "             H           H                         H                            ", //10
         "             H     ========<<<==             =============                      ", //11
         "             H                                         H                        ", //12
         "             H                                         H                        ", //13
         "     ===========>>>=======                          ========================    ", //14 
         "                        H                                         H             ", //15
         "                        H                                         H             ", //16
         "         ======================              ==========================         ", //17
         "                   H                              H                             ", //18
         "                   H                              H                             ", //19
         "               ======================================<<<                        ", //20
         "                                   H                                            ", //21
         "                                   H                                            ", //22
         "                                   H                                            ", //23                                                
         "================================================================================"  //24
    };

    char currentBoard[SCREEN_BOUNDARIES::MAX_Y][SCREEN_BOUNDARIES::MAX_X + 1]; // +1 for null terminator

public:
    /**
     * @brief Initializes the game board by resetting it and setting the locations of Paulina and Donkey Kong.
     * The `currentBoard` is updated to place Donkey Kong and Paulina at their respective positions,
     * as defined in the `CHARACTER_POSITIONS` structure.
     */
    Board() {
        reset();
        currentBoard[CHARACTER_POSITIONS::donkeyKong.getY()][CHARACTER_POSITIONS::donkeyKong.getX()] = static_cast<char>(ENTITIES_CHARACTERS::DONKEY_KONG);
        currentBoard[CHARACTER_POSITIONS::paulina.getY()][CHARACTER_POSITIONS::paulina.getX()] = static_cast<char>(ENTITIES_CHARACTERS::PAULINA);
    }

    void reset();
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
        return CHARACTER_POSITIONS::paulina;
    }
    Point getDonkeyKong() const{
        return CHARACTER_POSITIONS::donkeyKong;
    }
};

