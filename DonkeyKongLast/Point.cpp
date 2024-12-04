#include "Point.h"

void Point::move() {
	if (isNextPositionValid()) {
		x += dir.x;
		y += dir.y;
	}
}
