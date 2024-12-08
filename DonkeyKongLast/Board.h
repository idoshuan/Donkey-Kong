#pragma once
#include "Utils.h"
#include <iostream>

class Board {
    static constexpr int MAX_X = 80;
    static constexpr int MAX_Y = 25;
    static constexpr int MIN_X = 0;
    static constexpr int MIN_Y = 0;

    static constexpr char FLOOR = '=';  // Representing floor
    static constexpr char LADDER = 'H'; // Representing ladder
    static constexpr char LEFT_FLOOR = '<'; // Representing left floor
    static constexpr char RIGHT_FLOOR = '>'; // Representing right floor

    const char* originalBoard[MAX_Y] = {
        //          1         2         3         4         5         6         7
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789 
         "                                      $                                         ", //0
         "             =======================================================            ", //1
         "                                      &                                         ", //2
         "                      <<<<<<<<<<<<<<<<=>>>>>>>>>>>>>>>>                         ", //3
         "                                                                                ", //4
         "    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>                                          ", //5
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
public:
    Board() {
        reset();
        print();
    }

    void reset();
    void print() const;
    char getChar(int x, int y) const {
        return currentBoard[y][x];
    }
    bool isValidPosition(int x, int y) const {
        char ch = getChar(x, y);
        return(x >= MIN_X && x < MAX_X && y >= MIN_Y && y < MAX_Y && ch != FLOOR && ch != LEFT_FLOOR && ch != RIGHT_FLOOR);
    }
    bool isLadder(int x, int y) const {
        return getChar(x, y) == 'H';
    }
    bool isFloorBelow(int x, int y) const {
        char ch = getChar(x, y + 1);
        return ch == '=' || ch == '>' || ch == '<';
    }

};

