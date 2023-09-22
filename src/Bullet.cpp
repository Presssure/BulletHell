#include "Bullet.h"
Bullet::Bullet(Vector direction, Vector position, int angle) {
	this->alive = true;
	this->speed = 300;
	this->direction = direction;
	this->position = position;
	this->angle = angle;
}