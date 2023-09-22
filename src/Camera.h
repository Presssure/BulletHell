#pragma once
#include "Vector.h"
#include "Math.h"
class Camera {
public:
	Vector position;
	Vector direction;
	Vector forward;
	Vector right;
	Vector up;
	float pitch;
	float yaw;
	float roll;
	Camera();
};
