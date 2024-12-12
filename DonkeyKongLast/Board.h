#pragma once
#include "Utils.h"
#include "Point.h"
#include "GameConfig.h"
#include <iostream>

class Board {
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr int MIN_X = 0;
    static constexpr int MIN_Y = 0;
    static constexpr int PAULINA_X = 38;
    static constexpr int PAULINA_Y = 0;
    static constexpr int DONKEYKONG_X = 39;
    static constexpr int DONKEYKONG_Y = 2;

    static constexpr char FLOOR = '=';  // Representing floor
    static constexpr char LADDER = 'H'; // Representing ladder
    static constexpr char LEFT_FLOOR = '<'; // Representing left floor
    static constexpr char RIGHT_FLOOR = '>'; // Representing right floor

    const char* originalBoard[MAX_Y] = {
        //          1         2         3         4         5         6         7
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789 
         "Lives:                                                                          ", //0
         "             =======================================================            ", //1
         "               H                                                H               ", //2
         "               H       <<<<<<<<<<<<<<<<=>>>>>>>>>>>>>>>>        H               ", //3
         "               H                                                H               ", //4
         "    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>                          H               ", //5
         "    H                                              <<<<<<<<<<<<<<<<<            ", //6
         "    H                                              H                            ", //7
         "===========================                        H                            ", //8
         "             H           H                         H                            ", //9
         "             H           H                         H                            ", //10
         "             H     =======================       =========                      ", //11
         "             H                                         H                        ", //12
         "             H                                         H                        ", //13
         "     >>>>>>>>>>>>>>>>>>>>>                          ========================    ", //14 
         "                        H                                         H             ", //15
         "                        H                                         H             ", //16
         "         ======================              ==========================         ", //17
         "                   H                              H                             ", //18
         "                   H                              H                             ", //19
         "               =========================================                        ", //20
         "                                   H                                            ", //21
         "                                   H                                            ", //22
         "                                   H                                            ", //23                                                
         "================================================================================"  //24
    };
    char currentBoard[MAX_Y][MAX_X + 1]; // +1 for null terminator

    Point paulina;
    Point donkeyKong;

public:
    Board() :paulina(PAULINA_X, PAULINA_Y), donkeyKong(DONKEYKONG_X, DONKEYKONG_Y) {
        reset();
    }

    void reset();
    void print() const;
    char getChar(Point p) const {
        return currentBoard[p.getY()][p.getX()];
    }
    bool isValidPosition(Point p) const {
        char ch = getChar(p);
        int x = p.getX();
        int y = p.getY();
        return(x >= MIN_X && x < MAX_X && y >= MIN_Y && y < MAX_Y && ch != FLOOR && ch != LEFT_FLOOR && ch != RIGHT_FLOOR);
    }
    bool isLadder(Point p) const {
        return getChar(p) == BOARD_CHARACTERS::LADDER;
    }
    bool isFloorBelow(Point p) const {
        char ch = getChar(Point(p.getX(), p.getY() + 1));
        return ch == '=' || ch == '>' || ch == '<';
    }
    int getMaxX() const{
        return MAX_X;
    }
    int getMinX() const{
        return MIN_X;
    }

    Point getPaulina() {
        return paulina;
    }
    Point getDonkeyKong() {
        return donkeyKong;
    }
};

