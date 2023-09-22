#pragma once
#include <vector>
#include "Vector.h"
#include "Config.h"
#include <string>
#include <iostream>
class Arena {
public:
	float mat_diffuse[4]= { 0.0, 0.3, 0.8, 1.0 };
	float mat_specular[4]= { 1.0, 1.0, 1.0, 1.0 };
	float mat_shininess[1]={ 100.0 };
	float mat_ambient[4]= { 0.2, 0.2, 0.2, 1.0 };
	Vector front[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	Vector back[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	Vector left[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	Vector right[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	Vector top[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	Vector bottom[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
	void CreateArena(std::string place, Vector wall[][WALL_DIVISIONS + 1]);
	//Arena(int size, float xPos, float yPos, int width, int height, bool danger);
};

