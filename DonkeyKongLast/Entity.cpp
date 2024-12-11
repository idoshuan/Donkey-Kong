#include "Entity.h"

void Entity::move() {
	if (isNextPositionValid()) {
		setX(getX() + dir.x);
		setY(getY() + dir.y);
	}
	else {
		stop();
	}
}