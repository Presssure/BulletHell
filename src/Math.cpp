#include "Math.h"

float degToRadians(float angle) {
	float retVal = angle * 3.14159 / 180.0;
	return retVal;
}

Vector scalarMultiply(Vector direction, Vector position, Vector velocity, float dt) {
	float x = direction.x * velocity.x * dt;
	float y = direction.y * velocity.y * dt;
	float z = direction.z * velocity.z * dt;
	//std::cout << "start" << std::endl;
	//std::cout << direction.x << std::endl;
	//std::cout << velocity.x << std::endl;
	//std::cout << dt << std::endl;
	//std::cout << "dx: " << x << " dy: " << y << " dz: " << z << std::endl;
	x += position.x;
	y += position.y;
	z += position.z;
	//std::cout << "px: " << x << " py: " << y << " pz: " << z << std::endl;
	Vector temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	//std::cout << temp.x << " y: " << temp.y << " z: " << temp.z << std::endl;
	return temp;
}

Vector unitVector(Vector vector, float length) {
	Vector normalised;
	normalised.x = vector.x / length;
	normalised.y = vector.y / length;
	normalised.z = vector.y / length;
	return normalised;
}

float lengthVector(Vector vector) {
	float sqX = vector.x * vector.x;
	float sqY = vector.y * vector.y;
	float sqZ = vector.z * vector.z;
	float total = sqX + sqY + sqZ;
	return sqrt(total);
}

bool findCollision(Vector v1, Vector v2, float radius1, float radius2) {
	bool collision = false;
	float distX = v1.x - v2.x;
	float distY = v1.y - v2.y;
	float distZ = v1.z - v2.z;
	float distance = sqrt((distX * distX) + (distY * distY)+(distZ*distZ));
	//std::cout << distance << " <= " << radius1 + radius2 << std::endl;
	if (distance <= radius1 + radius2) {
		collision = true;
	}
	return collision;
}

void calcVector(float angle, Vector* vector)
{
	
	vector->y = cos(degToRadians(angle));
	vector->x = sin(degToRadians(angle));

	std::cout << "This is the angle: " << angle << "x calc:=" << cos(degToRadians(angle)) << "y calc:= " << sin(degToRadians(angle)) << std::endl;

}

void calcVectorZ(float angle, Vector * vector)
{

	vector->y = cos(degToRadians(angle));

	std::cout << "This is the angle: " << angle << "x calc:=" << cos(degToRadians(angle)) << "y calc:= " << sin(degToRadians(angle)) << std::endl;

}

float dotProduct(Vector v1, Vector v2) {
	float ab_x = v1.x * v2.x;
	float ab_y = v1.y * v2.y;
	float ab = ab_x + ab_y;
	float magA = sqrt((v1.x * v1.x) + (v1.y * v1.y));
	float magB = sqrt((v2.x * v2.x) + (v2.y * v2.y));
	return acos((ab) / (magA * magB));
}