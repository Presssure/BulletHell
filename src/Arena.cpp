#include "Arena.h"

//Arena::Arena(int size, float xPos, float yPos, int width, int height, bool danger) {
//	this->size = size;
//	this->xPos = xPos;
//	this->yPos = yPos;
//	this->width = width;
//	this->height = height;
//	this->danger = danger;
//}

void Arena::CreateArena(std::string place, Vector wall[][WALL_DIVISIONS + 1]) {
    int x_min = WALL_LEFT;
    int x_max;
    int y_min;
    int y_max;
    int z_min;
    int z_max;
    //if (place == "front") {
    x_min = -15;
    x_max = 15;
    y_min = -15;
    y_max = 15;
    z_min = -15;
    z_max = 15;
    //}
    // calculate distance between each vertex
    float x_step = (x_max - x_min) / (float)WALL_DIVISIONS;
    float y_step = (y_max - y_min) / (float)WALL_DIVISIONS;
    float z_step = (z_max - z_min) / (float)WALL_DIVISIONS;
    Vector vertex;
    std::cout << place << std::endl;
    if (place == "front") {
        std::cout << "reached here" << std::endl;
        vertex.z = z_min;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.y = y_min + row * y_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.x = x_min + col * x_step;
                wall[row][col] = vertex;
            }
        }
        std::cout << "ended the if" << std::endl;
    }
    else if (place == "back") {
        vertex.z = z_max;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.y = y_min + row * y_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.x = x_min + col * x_step;
                wall[row][col] = vertex;
            }
        }
    }
    else if (place == "left") {
        vertex.x = x_min;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.y = y_min + row * y_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.z = z_min + col * z_step;
                wall[row][col] = vertex;
            }
        }
    }
    else if (place == "right") {
        vertex.x = x_max;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.y = y_min + row * y_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.z = z_min + col * z_step;
                wall[row][col] = vertex;
            }
        }
    }
    else if (place == "top") {
        vertex.y = y_max;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.z = z_min + row * z_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.x = x_min + col * x_step;
                wall[row][col] = vertex;
                //std::cout << "x: " << vertex.x << "   z: " << vertex.z << std::endl;
            }
        }
    }
    else if (place == "bottom") {
        vertex.y = y_min;
        // fill the array of vertices
        for (int row = 0; row <= WALL_DIVISIONS; ++row) {
            vertex.z = z_min + row * z_step;
            for (int col = 0; col <= WALL_DIVISIONS; ++col) {
                vertex.x = x_min + col * x_step;
                wall[row][col] = vertex;
            }
        }
    }
}