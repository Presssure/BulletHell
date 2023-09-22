#pragma once
#include "Vector.h"
#include "Config.h"
#include "Math.h"
class Asteroid {
public:
	float mat_diffuse[4] = { 0.0, 0.3, 0.8, 1.0 };
	float mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_shininess[1] = { 100.0 };
	float mat_ambient[4] = { 0.1, 0.1, 0.1, 1.0 };


	bool in;
	Vector position;
	Vector direction;
	int speed;
	float radius;
	bool clockwise;
	int rotationAngle;
	int rotationDelta;
	int hitPoints;
	int size;
	int texture;
	Vector verticies[ASTEROID_DIVISIONS+1][ASTEROID_DIVISIONS+1];
	Asteroid(Vector position, Vector direction, float speed, float radius, bool clockwise,
		int rotationAngle, int rotationDelta, int texture);
	void initVerticies();
	void initVerticiesRadius();
};