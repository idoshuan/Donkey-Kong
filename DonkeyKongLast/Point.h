#pragma once

/**
 * @brief Represents a 2D point with X and Y coordinates.
 */

 // ------------------- Class Declaration -------------------
class Point {
	int x, y;
public:
    // ------------------- Constructor -------------------
    Point() = default;
	Point(int x, int y) :x(x), y(y) {}

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Setters
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

    // Operators
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};
