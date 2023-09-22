#pragma once
#include "Vector.h"
#include "Config.h"
class Ship {
public:
	float mat_diffuse[4] = { 0.0, 0.3, 0.8, 1.0 };
	float mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_shininess[1] = { 100.0 };
	float mat_ambient[4] = { 0.5, 0.5, 0.5, 1.0 };
	float wing_diffuse[4]= { 0.0, 0.5, 0.5, 1.0 };
	float wing_specular[4]= { 0.5, 0.5, 0.5, 1.0 };

	Vector position;
	Vector direction;
	Vector front;
	Vector up;
	Vector left;
	bool close[6];
	bool alive;
	float radius;
	void checkShip();
};