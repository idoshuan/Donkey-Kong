#pragma once
#include "Board.h"
#include "Point.h"
#include "GameConfig.h"
#include <iostream>

class Entity {
private:
    // Direction struct is now encapsulated inside Entity class
    struct Direction {
        int x, y;
    };

    // Member variables
    ENTITIES_CHARACTERS ch;           
    Point point;      
    Board* pBoard;    
    Direction dir;    

    // Private method to draw the character at its position
    void drawCharacter(char c) const {
        gotoxy(getX(), getY());
        std::cout << c;
    }

public:
    // Predefined direction constants
    static constexpr Direction DOWN = { 0, 1 };
    static constexpr Direction LEFT = { -1, 0 };
    static constexpr Direction RIGHT = { 1, 0 };
    static constexpr Direction NONE = { 0, 0 };

    // Constructor with default values
    Entity(ENTITIES_CHARACTERS ch, int x, int y, Board* pBoard = nullptr, Direction dir = NONE) : ch(ch), point(x, y), pBoard(pBoard), dir(dir) {}

    // Accessors for position
    int getX() const { return point.getX(); }
    int getY() const { return point.getY(); }

    // Setters for position
    void setX(int newX) { point.setX(newX); }
    void setY(int newY) { point.setY(newY); }

    // Accessor for direction
    Direction getDir() const { return dir; }
    int getDirX() const { return dir.x; }
    int getDirY() const { return dir.y; }

    // Setters for direction
    void setDir(Direction newDir) { dir = newDir; }
    void setDirX(int newX) { dir.x = newX; }
    void setDirY(int newY) { dir.y = newY; }

    // Set the board the entity is on
    void setBoard(Board& board) { pBoard = &board; }
    Board* getBoard() { return pBoard; }

    // Check if entity is on the floor or ladder
    bool isOnFloor() const { return pBoard->isFloorBelow(point); }
    bool isOnLadder() const { return pBoard->getChar(point) == BOARD_CHARACTERS::LADDER; }

    // Check if the next position is valid
    bool isNextPositionValid() const {
        return pBoard->isValidPosition(Point(getX() + dir.x, getY() + dir.y));
    }

    // Drawing and erasing entity
    void draw() const {
        drawCharacter(static_cast<char>(ch));
    }
    void erase() {
        char backgroundChar = pBoard->getChar(Point(getX(), getY()));
        drawCharacter(backgroundChar);
    }

    // Special functions
    void move();
    char floorDir() {
        return pBoard->getChar(Point(getX(), getY() + 1));
    }

    // Stop the entity's movement (set direction to NONE)
    void stop() { dir = NONE; }

};
