#pragma once

class Point {
	int x, y;
public:
	Point(int x, int y) :x(x), y(y) {}

	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	void setX(int newX) {
		x = newX;
	}
	void setY(int newY) {
		y = newY;
	}
	
	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}

};

