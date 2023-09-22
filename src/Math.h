#pragma once
#ifndef MATH_H

#include <iostream>
#include <math.h>
#include "Vector.h"

Vector scalarMultiply(Vector direction, Vector position, Vector velocity, float dt);
void calcVector(float angle, Vector *vector);
void calcVectorZ(float angle, Vector* vector);
float degToRadians(float angle);
float lengthVector(Vector vector);
Vector unitVector(Vector vector, float length);
bool findCollision(Vector v1, Vector v2, float radius1, float radius2);
float dotProduct(Vector v1, Vector v2);
#endif // !MATH_H
