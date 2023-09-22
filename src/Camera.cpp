#include "Camera.h"

Camera::Camera() {
	Vector tempPos;
	tempPos.x = 0;
	tempPos.y = 20;
	tempPos.z = 0;
	Vector tempDir;
	tempDir.x = 0;
	tempDir.y = 0;
	tempDir.z = 1;
	this->position = tempPos;
	this->direction = tempDir;
	this->pitch = 0;
	this->yaw = 0;
	this->roll = 0;
}



