#include "Asteroid.h"

Asteroid::Asteroid(Vector position, Vector direction, float speed, float radius, bool clockwise,
    int rotationAngle, int rotationDelta, int texture) {
    this->position = position;
    this->direction = direction;
    this->speed = speed;
    this-> radius = radius;
    this->clockwise = clockwise;
    this->rotationAngle = rotationAngle;
    this->rotationDelta = rotationDelta;
    this->texture = texture;
    this->hitPoints = 2;
    this->in = false;
}

void Asteroid::initVerticies()
{
    float theta, phi;
    Vector vertex;
    float radius = 1;
    float step_theta = 2.0 * M_PI / ASTEROID_DIVISIONS;
    float step_phi = M_PI / ASTEROID_DIVISIONS;
    phi = 0;
    for (int i = 0; i <= ASTEROID_DIVISIONS; i++, phi += step_phi)
    {
        theta = 0;
        for (int j = 0; j <= ASTEROID_DIVISIONS; j++, theta += step_theta)
        {
            vertex.x = radius * sinf(phi) * cosf(theta);
            vertex.y = radius * cosf(phi);
            vertex.z = radius * sinf(phi) * sinf(theta);
            this->verticies[i][j] = vertex;
        }
    }
}

void Asteroid::initVerticiesRadius() {
    float theta, phi;
    Vector vertex;
    float radius = 1;
    float step_theta = 2.0 * M_PI / ASTEROID_DIVISIONS;
    float step_phi = M_PI / ASTEROID_DIVISIONS;
    phi = 0;
    for (int i = 0; i <= ASTEROID_DIVISIONS; i++, phi += step_phi)
    {
        theta = 0;
        for (int j = 0; j <= ASTEROID_DIVISIONS; j++, theta += step_theta)
        {
            vertex.x = this->radius * sinf(phi) * cosf(theta);
            vertex.y = this->radius * cosf(phi);
            vertex.z = this->radius * sinf(phi) * sinf(theta);
            this->verticies[i][j] = vertex;
        }
    }
}
