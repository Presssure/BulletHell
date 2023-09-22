#include "Ship.h"

void Ship::checkShip() {
	for (int i = 0; i < 6; i++) {
		close[i] = false;
	}
	if (this->position.z > WALL_LIMIT) {
		close[FRONT] = true;
	}
	if (this->position.z < -WALL_LIMIT) {
		close[BACK] = true;
	}
	if (this->position.x < -WALL_LIMIT) {
		close[LEFT] = true;
	}
	if (this->position.x > WALL_LIMIT) {
		close[RIGHT] = true;
	}
	if (this->position.y > WALL_LIMIT) {
		close[TOP] = true;
	}
	if (this->position.y < -WALL_LIMIT) {
		close[BOTTOM] = true;
	}
}