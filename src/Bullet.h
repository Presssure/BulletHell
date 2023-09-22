#pragma once
#include "Vector.h"
class Bullet {
public:
	bool alive;
	int speed;
	int angle;
	Vector direction;
	Vector position;
	Bullet(Vector direction, Vector position, int angle);
};