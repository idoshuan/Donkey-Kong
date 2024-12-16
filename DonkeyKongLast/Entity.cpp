#include "Entity.h"

// ------------------- Action Functions -------------------

/**
 * @brief Moves the entity to the next position if valid. If not valid, stops the entity.
 */
void Entity::move() {
    if (isNextPositionValid()) {
        setX(getX() + dir.x);
        setY(getY() + dir.y);
    }
    else {
        stop();
    }
}
