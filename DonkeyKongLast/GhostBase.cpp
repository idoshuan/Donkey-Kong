#include "GhostBase.h"


/**
 * @brief Randomly changes the ghost's direction.
 * Introduces a 0.05 probability for the ghost to turn around.
 */
void GhostBase::setRandDir() {
	if (getRandomDouble() <= turnProbability) {
		turnAround();
	}
}