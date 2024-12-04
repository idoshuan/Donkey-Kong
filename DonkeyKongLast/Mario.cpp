#include "Mario.h"

void Mario::keyPressed(char key) {
	for (size_t k = 0; k < numKeys; k++) {
		if (std::tolower(key) == keys[k]) {
			if (k == 0) {
				//handleUp();
			}
			else if (k == 1) {
				//handleDown();
			}
			else {
				return;
			}
		}
	}
}