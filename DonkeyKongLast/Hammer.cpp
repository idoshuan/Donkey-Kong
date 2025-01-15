#include "Hammer.h"

/**
 * @brief Handles the hammer's movement with Mario.
 * Updates the hammer's position based on Mario's direction and ensures it stays
 * within valid positions on the board.
 */
void Hammer::move() {
	if (mario.getDirX() != Entity::X_NONE) {
		if (mario.getBoard()->isValidPosition({ pos.getX() + mario.getDirX(), mario.getY() })) {
			pos.setX(mario.getX() + mario.getDirX());
		}
		else {
			pos.setX(mario.getX() - mario.getDirX());
		}
	}
	pos.setY(mario.getY());
}
