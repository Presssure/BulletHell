#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Vector.h"
#include "Arena.h"
#include "Config.h"
#include <string>
#include "Math.h"
#include "Camera.h"
#include "Ship.h"
#include "Material.h"
#include <vector>
#include "Asteroid.h"
#include "bullet.h"
#if _WIN32
#include <windows.h>
#endif
#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include "stb_image.h"
float g_last_time = 0.0;

bool g_show_axes = false;
float g_camera_position_x, g_camera_position_y, g_camera_position_z;
float g_camera_rotation = 90.0;
float g_platform_rotation = 0.0;
float g_cannon_rotation = 0.0;
float g_camera_height = 20.0;
float dt = 0;

// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
// Get mouse position
int xpos, ypos, zpos;
int height = 2560;
int width = 1440;
Camera camera;
Ship ship;
GLuint skybox[6];
char input;
std::vector <Asteroid> asteroids;
Asteroid *spawnPoint;
GLuint asteroidTexture[4];
std::vector<Bullet> bullets;
int waves = 1;
bool completedWave = false;
float g_x=0, g_y = 0;
bool die = false;
void print(Vector vector) {
    std::cout << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << std::endl;
}
void add_barrel_light()
{
    float ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float position[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glEnable(GL_LIGHT1);

    // add point for reference/troubleshooting
    glDisable(GL_LIGHTING);
    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glEnd();
    glEnable(GL_LIGHTING);
}

#define CAMERA_DISTANCE 25.0
#define CAMERA_HEIGHT 20.0

void update_camera_position()
{

    /*g_camera_position_x = CAMERA_DISTANCE * cos(g_camera_rotation * M_PI / 180.0);
    g_camera_position_z = CAMERA_DISTANCE * sin(g_camera_rotation * M_PI / 180.0);*/
    g_camera_position_x = horizontalAngle;
    g_camera_position_z = verticalAngle;
    g_camera_position_y = CAMERA_HEIGHT;
    //g_camera_position_y = CAMERA_DISTANCE * sin(g_camera_height * M_PI / 180.0);
}
void cameraTest() {
    glRotatef(camera.yaw, 1.0, 0.0, 0.0);
    glRotatef(camera.pitch, 0.0, 1.0, 0.0);
    glTranslated(camera.position.x, camera.position.y, camera.position.z);
}
float deltaAngle = 0.0f;
int xOrigin = -1;
int oldx = 0;
int oldy = 0;

//position
//pitch
//roll
//yaw
//3 vectors forward, up and right
//3 functions to add pitch, roll and yaw

void moveMouse(int x, int y){

    verticalAngle += x - oldx;
    horizontalAngle += y - oldy;
    g_camera_position_y = CAMERA_HEIGHT;
    
    float angle = x - oldx / 360;
    float yAngle = y - oldy / 360;
    angle = degToRadians(angle);
    yAngle = degToRadians(yAngle);
    camera.pitch = cos(angle)*sin(yAngle);
    camera.yaw = cos(yAngle);
    camera.roll = sin(angle)*sin(yAngle);
    camera.direction.x = camera.pitch;
    camera.direction.y = camera.yaw;
    camera.direction.z = camera.roll;

    oldx = x;
    oldy = y;
    //std::cout << camera.direction.x << "  y: " << camera.direction.y << std::endl;
    //std::cout << verticalAngle << "  y: " << horizontalAngle << std::endl;
}


// load a texture from file using the stb_image library
uint32_t loadTexture(const char* filename) {
    int width, height, components;
    unsigned char* data = stbi_load(filename, &width, &height, &components, STBI_rgb);

    glPushAttrib(GL_TEXTURE_BIT);

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    //      glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    glPopAttrib();
    return id;
}

void loadSkybox() {
    skybox[0] = loadTexture("../skybox/front.png");
    skybox[1] = loadTexture("../skybox/back.png");
    skybox[2] = loadTexture("../skybox/left.png");
    skybox[3] = loadTexture("../skybox/right.png");
    skybox[4] = loadTexture("../skybox/top.png");
    skybox[5] = loadTexture("../skybox/bottom.png");

}
void drawBullet() {
    
    float mat_diffuse[4] = { 0.0, 0.3, 0.8, 1.0 };
    float mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
    float mat_shininess[1] = { 100.0 };
    float mat_ambient[4] = { 0.2, 0.2, 0.2, 1.0 };
    for (int i = 0; i < bullets.size();i++) {
        std::cout << i << std::endl;
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glTranslatef(bullets.at(i).position.x, bullets.at(i).position.y, bullets.at(i).position.z);
        //print(bullets.at(i).position);
        //glTranslatef(0,0,0);
        glRotatef(-bullets.at(i).angle, 0, 0, 1);
        glScalef(1, 1, 1);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
}
void bulletCollide(Bullet bullet) {
    if (bullet.position.x > ARENA_SIZE) {
        bullet.alive = false;
    }
    if (bullet.position.y > ARENA_SIZE) {
        bullet.alive = false;
    }
    if (bullet.position.z > ARENA_SIZE) {
        bullet.alive = false;
    }
    if (bullet.position.x < -ARENA_SIZE) {
        bullet.alive = false;
    }
    if (bullet.position.y < -ARENA_SIZE) {
        bullet.alive = false;
    }
    if (bullet.position.z < -ARENA_SIZE) {
        bullet.alive = false;
    }
    for (int i = 0; i < asteroids.size();i++) {
        bool ret = false;
        Vector vecd;
        vecd.x = bullet.position.x - asteroids.at(i).position.x;
        vecd.y = bullet.position.y - asteroids.at(i).position.y;
        vecd.z = bullet.position.z - asteroids.at(i).position.z;
        float distance = sqrt((vecd.x * vecd.x) + (vecd.y * vecd.y) + (vecd.z * vecd.z));
        float radius = 1 + asteroids.at(i).radius;
        if (distance < radius) {
            ret = true;
            //std::cout << "it's true" << std::endl;
        }
        if (ret) {
            bullet.alive = false;
            asteroids.erase(asteroids.begin() + i);
        }
    }
}
void checkBullet() {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets.at(i).alive) {
            bulletCollide(bullets.at(i));
        }
        else {
            bullets.erase(bullets.begin() + i);
        }
        
    }
}
void displaySkybox() {
    //glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(0, 0, camera.position.z);
    glRotatef(camera.pitch, 1, 0,0);
    glRotatef(camera.yaw, 0, 1, 0);
    //glRotatef(camera.roll, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor4f(1, 1, 1, 1);

    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();
    //glEnable(GL_LIGHTING);
    //// Render the left quad
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();

    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);

    glEnd();

    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();

    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1, 0); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    // Restore enable bits and matrix
    //glPopAttrib();
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}
void initAssets() {
    
    asteroidTexture[0] = loadTexture("../assets/asteroids/asteroid1.jpg");
    asteroidTexture[1] = loadTexture("../assets/asteroids/asteroid2.jpg");
    asteroidTexture[2] = loadTexture("../assets/asteroids/asteroid3.jpg");
    asteroidTexture[3] = loadTexture("../assets/asteroids/asteroid4.jpg");
}
Vector getRandomPosition() {
    int temp = rand() % (ASTEROID_DIVISIONS);

    return spawnPoint->verticies[temp][temp];
}
void updateAsteroid(std::vector<Asteroid>* asteroids) {
    for (int i = 0; i < asteroids->size(); i++) {
        Vector tempVector;
        tempVector.x = asteroids->at(i).speed + 5;
        tempVector.y = asteroids->at(i).speed + 5;
        tempVector.z = asteroids->at(i).speed + 5;
        //std::cout << asteroids->at(i).speed << std::endl;
        asteroids->at(i).position = scalarMultiply(asteroids->at(i).direction, asteroids->at(i).position, tempVector, dt);
        int clock = 1;
        if (asteroids->at(i).clockwise) {
            clock = -1;
        }
        asteroids->at(i).rotationAngle += asteroids->at(i).rotationDelta * dt * clock;

        //std::cout << " no: " << i << " x: " << asteroids.at(i).position.x << " y: "<<asteroids.at(i).position.y << " z: "<<asteroids.at(i).position.x << std::endl;
    }
}
void loadAsteroids() {
    Vector temp;
    temp.x = 0;
    temp.y = 0;
    temp.z = 0;
    int randomTexture = rand() % 4;
    int randomSpeed=rand()%10;
    float randomRadius= rand() % (5-2+1)+2;

    bool clockwise=rand()%2;
    
    int randomDelta = rand() % (180-360)+180;
    Vector ran = getRandomPosition();
    Vector pos;
    pos.x = camera.position.x-ran.x;
    pos.y = camera.position.y-ran.y;
    pos.z = camera.position.z-ran.z;
    float unitLength = lengthVector(pos);
    Vector dir = unitVector(pos, unitLength);
    Asteroid* asteroid = new Asteroid(ran, dir, randomSpeed, randomRadius, clockwise, 0, randomDelta, randomTexture);
    asteroid->initVerticies();
    /*for (int i = 0; i < ASTEROID_DIVISIONS + 1;i++) {
        for (int j = 0;j < ASTEROID_DIVISIONS + 1;j++) {
            std::cout << asteroid->verticies[i][j].x <<" y:"<< asteroid->verticies[i][j].y<<" z:"<< asteroid->verticies[i][j].z<< std::endl;
        }
    }*/
    asteroids.push_back(*asteroid);
}
void loadBullet() {
    Bullet* bullet = new Bullet(camera.direction, camera.position, 0);
    bullets.push_back(*bullet);
}
void on_mousepress(int key, int state, int x, int y) {
    switch (state) {
    case(GLUT_DOWN):
        loadBullet();
        break;
    }
}


bool checkCollision(Asteroid s1, Asteroid s2) {
    bool ret = false;
    Vector vecd;
    vecd.x = s1.position.x - s2.position.x;
    vecd.y = s1.position.y - s2.position.y;
    vecd.z = s1.position.z - s2.position.z;
    float distance = sqrt((vecd.x * vecd.x) + (vecd.y * vecd.y) + (vecd.z * vecd.z));
    float radius = s1.radius + s2.radius;
    if (distance < radius) {
        ret = true;
        //std::cout << "it's true" << std::endl;
    }
    return ret;
}
void checkInside() {
    for (int i = 0; i < asteroids.size();i++) {
        if (!asteroids.at(i).in) {
            if (asteroids.at(i).position.x + asteroids.at(i).radius < ARENA_SIZE &&
                asteroids.at(i).position.x - asteroids.at(i).radius > -ARENA_SIZE &&
                asteroids.at(i).position.y + asteroids.at(i).radius < ARENA_SIZE &&
                asteroids.at(i).position.y - asteroids.at(i).radius > -ARENA_SIZE &&
                asteroids.at(i).position.z + asteroids.at(i).radius < ARENA_SIZE &&
                asteroids.at(i).position.z - asteroids.at(i).radius > -ARENA_SIZE) {
                asteroids.at(i).in = true;
            }
        }

    }
}
void checkAsteroid() {
    for (int i = 0; i < asteroids.size();i++) {
        if (asteroids.at(i).in) {
            if (asteroids.at(i).position.x + asteroids.at(i).radius >= ARENA_SIZE) {
                asteroids.at(i).direction.x *= -1;
            }
            if (asteroids.at(i).position.x - asteroids.at(i).radius <= -ARENA_SIZE) {
                asteroids.at(i).direction.x *= -1;
            }
            if (asteroids.at(i).position.y + asteroids.at(i).radius >= ARENA_SIZE) {
                asteroids.at(i).direction.y *= -1;
            }
            if (asteroids.at(i).position.y - asteroids.at(i).radius <= -ARENA_SIZE) {
                asteroids.at(i).direction.y *= -1;
            }
            if (asteroids.at(i).position.z + asteroids.at(i).radius >= ARENA_SIZE) {
                asteroids.at(i).direction.z *= -1;
            }
            if (asteroids.at(i).position.z - asteroids.at(i).radius <= -ARENA_SIZE) {
                asteroids.at(i).direction.z *= -1;
            }
            for (int j = 0; j < asteroids.size(); j++) {
                if (i != j) {
                    if (checkCollision(asteroids.at(i), asteroids.at(j))) {
                        std::cout << i << " 1" << std::endl;
                        print(asteroids.at(i).direction);
                        asteroids.at(i).direction.x *= -1;
                        asteroids.at(i).direction.y *= -1;
                        asteroids.at(i).direction.z *= -1;
                        std::cout << i << " 2" << std::endl;
                        print(asteroids.at(i).direction);
                        asteroids.at(j).direction.x *= -1;
                        asteroids.at(j).direction.y *= -1;
                        asteroids.at(j).direction.z *= -1;
                        updateAsteroid(&asteroids);
                    }
                }
            }
        }
    }
}

void randomPosition() {
    Vector temp;
    temp.x = 0;
    temp.y = 0;
    temp.z = 0;
    spawnPoint=new Asteroid(temp,temp,0,300,true, 0,0,0);
    spawnPoint->initVerticiesRadius();
    //for (int i = 0; i < ASTEROID_DIVISIONS;i++) {
    //    for (int j = 0; j < ASTEROID_DIVISIONS;j++) {
    //        std::cout << spawnPoint->verticies[i][j].x << " y: " << spawnPoint->verticies[i][j].y <<std::endl;
    //    }
    //}
}

void generateAsteroids() {
    for (int i = 0; i < waves; i++) {
        loadAsteroids();
    }
}
void drawRing() {

    glMaterialfv(GL_FRONT, GL_SPECULAR, ship.mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, ship.mat_shininess);

    // position and draw
   
        glPushMatrix();
        glRotatef(g_platform_rotation, 0.0, 1.0, 0.0);
        glTranslatef(spawnPoint->position.x, spawnPoint->position.y, spawnPoint->position.z);
        glBindTexture(GL_TEXTURE_2D, asteroidTexture[0]);
        glScalef(1.0, 1.0, 1.0);
        //glutSolidCube(1.0);
        int i, j;
        Vector* v1, * v2;
        float u, v;
        Vector texture;
        for (j = 0; j < ASTEROID_DIVISIONS; j++) {    // divide by 2 for hemisphere
            glBegin(GL_TRIANGLE_STRIP);
            for (i = 0; i <= ASTEROID_DIVISIONS; i++) {
                v1 = &spawnPoint->verticies[i][j];
                v2 = &spawnPoint->verticies[i][j + 1];
                texture.x = asinf(v1->x) / M_PI + 0.5f;
                texture.y = asinf(v1->y) / M_PI + 0.5f;
                glTexCoord2f(texture.x, texture.y);
                glNormal3f(v1->x, v1->y, v1->z);
                glVertex3f(v1->x, v1->y, v1->z);
                texture.x = asinf(v2->x) / M_PI + 0.5f;
                texture.y = asinf(v2->y) / M_PI + 0.5f;
                glTexCoord2f(texture.x, texture.y);
                glNormal3f(v2->x, v2->y, v2->z);
                glVertex3f(v2->x, v2->y, v2->z);
            }
            glEnd();
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }



void drawAsteroids(std::vector<Asteroid>* asteroids)
{
    
    //glMaterialfv(GL_FRONT, GL_AMBIENT, ship.mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, ship.mat_diffuse);
    
    // position and draw
    for (int k = 0; k < asteroids->size(); k++) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, asteroids->at(k).mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, asteroids->at(k).mat_shininess);
        glMaterialfv(GL_FRONT, GL_SHININESS, asteroids->at(k).mat_ambient);
        glPushMatrix();
        
        glTranslatef(asteroids->at(k).position.x, asteroids->at(k).position.y, asteroids->at(k).position.z);
        glRotatef(asteroids->at(k).rotationAngle, 0.0, 1.0, 0.0);
        //std::cout << asteroids->at(k).position.x << " y: " << asteroids->at(k).position.y << " z: " << asteroids->at(k).position.z << std::endl;
        //glTranslatef(0, 0, 0);
        //std::cout << asteroids->at(k).position.x << " y: " << asteroids->at(k).position.y << " z: " << asteroids->at(k).position.z << std::endl;
        glBindTexture(GL_TEXTURE_2D, asteroidTexture[asteroids->at(k).texture]);
        //std::cout << asteroids->at(k).texture<<std::endl;
        glScalef(asteroids->at(k).radius, asteroids->at(k).radius, asteroids->at(k).radius);
        //glutSolidCube(1.0);
        int i, j;
        Vector* v1, * v2;
        float u, v;
        Vector texture;
        for (j = 0; j < ASTEROID_DIVISIONS; j++) {    // divide by 2 for hemisphere
            glBegin(GL_TRIANGLE_STRIP);
            for (i = 0; i <= ASTEROID_DIVISIONS; i++) {
                v1 = &asteroids->at(k).verticies[i][j];
                v2 = &asteroids->at(k).verticies[i][j + 1];
                texture.x = asinf(v1->x) / M_PI + 0.5f;
                //texture.x = atan2(v1->x, v1->z) / (2 * M_PI) + 0.5;
                texture.y = asinf(v1->y) / M_PI + 0.5;
                glTexCoord2f(texture.x, texture.y);
                glNormal3f(v1->x, v1->y, v1->z);
                glVertex3f(v1->x, v1->y, v1->z);
                //texture.x = atan2(v2->x, v2->z) / (2 * M_PI) + 0.5;
                texture.x = asinf(v2->x) / M_PI + 0.5f;
                texture.y = asinf(v2->y) / M_PI + 0.5f;
                glTexCoord2f(texture.x, texture.y);
                glNormal3f(v2->x, v2->y, v2->z);
                glVertex3f(v2->x, v2->y, v2->z);
                //std::cout << v1->x << " y: " << v1->y << " z: " << v1->z << std::endl;
            }
            glEnd();
        }
        
        glPopMatrix();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    }


void draw_ship() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ship.mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ship.mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ship.mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, ship.mat_shininess);

    glPushMatrix();
    glTranslatef(ship.position.x, ship.position.y, ship.position.z-20);
    glScalef(10, 10, 10);
    glRotatef(horizontalAngle, 1,0,0);
    glRotatef(verticalAngle, 0, 1, 0);
    //glRotatef(ship.direction.z, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(-0.5, 0.5, 0);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f(0.5, -0.5, 0);
    glVertex3f(0.5, 0.5, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, 0.5, 0);
    glVertex3f(0, 0, 1.5);
    glVertex3f(0.5, 0.5, 0);
    glEnd();
    float mat_diffuse[4] = { 0.0, 0.3, 0.8, 1.0 };
    float mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
    float mat_shininess[1] = { 100.0 };
    float mat_ambient[4] = { 0.1, 0.1, 0.1, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f(0, 0, 1.5);
    glVertex3f(0.5, -0.5, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, 0.5, 0);
    glVertex3f(0, 0, 1.5);
    glVertex3f(-0.5, -0.5, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(0.5, 0.5, 0);
    glVertex3f(0, 0, 1.5);
    glVertex3f(0.5, -0.5, 0);
    glEnd();
    glPopMatrix();
}

Vector wall_vertices[WALL_DIVISIONS + 1][WALL_DIVISIONS + 1];
Arena g_arena;

void draw_Arena(Arena* arena) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, arena->mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, arena->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, arena->mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, arena->mat_shininess);
    float pos = -1.0;
    float inc = 2.0/(float)WALL_DIVISIONS;
    float grey[4] = { 105, 105, 105 };
    float red[4] = { 1, 0, 0 };
    glPushMatrix();
    glTranslatef(0, 0, -ARENA_SIZE);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //front
    if (ship.close[BACK]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc) {
        glVertex3f(pos, -1.0, 0.0);
        glVertex3f(pos, 1.0, 0.0);
        glVertex3f(-1.0, pos, 0.0);
        glVertex3f(1.0, pos, 0.0);
    }
    glEnd();
    glPopMatrix();
    pos = -1.0;
    glPushMatrix();
    glTranslatef(-ARENA_SIZE, 0.0, 0.0);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //left
    if (ship.close[LEFT]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc)
    {
        glVertex3f(0.0, pos, -1.0);
        glVertex3f(0.0, pos, 1.0);
        glVertex3f(0.0, -1.0, pos);
        glVertex3f(0.0, 1.0, pos);
    }
    glEnd();
    glPopMatrix();
    pos = -1.0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //right
    if (ship.close[RIGHT]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc)
    {
        glVertex3f(1.0, pos, -1.0);
        glVertex3f(1.0, pos, 1.0);
        glVertex3f(1.0, -1.0, pos);
        glVertex3f(1.0, 1.0, pos);
    }
    glEnd();
    glPopMatrix();
    pos = -1.0;
    glPushMatrix();
    glTranslatef(0.0, -ARENA_SIZE, 0.0);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //bottom
    if (ship.close[BOTTOM]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc)
    {
        glVertex3f(-1.0, 0.0, pos);
        glVertex3f(1.0, 0.0, pos);
        glVertex3f(pos, 0.0, -1.0);
        glVertex3f(pos, 0.0, 1.0);
    }
    glEnd();
    glPopMatrix();
    pos = -1.0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //top
    if (ship.close[TOP]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc)
    {
        glVertex3f(-1.0, 1.0, pos);
        glVertex3f(1.0, 1.0, pos);
        glVertex3f(pos, 1.0, -1.0);
        glVertex3f(pos, 1.0, 1.0);
    }
    glEnd();
    glPopMatrix();
    pos = -1.0;
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE);
    glBegin(GL_LINES);
    //back
    if (ship.close[FRONT]) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    }
    for (int i = 0; i <= WALL_DIVISIONS; i++, pos += inc)
    {
        glVertex3f(pos, -1.0, 1.0);
        glVertex3f(pos, 1.0, 1.0);
        glVertex3f(-1.0, pos, 1.0);
        glVertex3f(1.0, pos, 1.0);
    }
    glEnd();
    glPopMatrix();
}


void draw_axes()
{
    if (!g_show_axes) return;

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-100.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, -100.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, -100.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 100.0);
    glEnd();
    glEnable(GL_LIGHTING);
}
//void checkAsteroid() {
//    bool dead = false;
//    
//    for (int i = 0;i < asteroids.size();i++) {
//        if (asteroids.at(i).position.x > 1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).position.x < -1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).position.y > 1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).position.x < -1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).position.z > 1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).position.x < -1000) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (asteroids.at(i).hitPoints < 1) {
//            dead = true;
//            std::cout << "Something" << std::endl;
//        }
//        if (dead) {
//            asteroids.erase(asteroids.begin() + i);
//        }
//    }
//}

void initShip() {
    Vector temp;
    temp.x = 1;
    temp.y = 1;
    temp.z = 1;
    Vector temp2;
    temp2.x = 0.001;
    temp2.y = 0.001;
    temp2.z = 0.001;
    ship.position = temp;
    ship.direction = temp2;
    temp2.z = -1;
    ship.front = temp2;
    temp2.z = 0.001;
    temp2.y = 1;
    ship.up = temp2;
    temp2.y = 0.001;
    temp2.x = 1;
    ship.left = temp2;
}
void updateShip(Ship* ship, Camera* camera) {
    /*ship->position.x = camera->position.x + camera->direction.x+5;
    ship->position.y = camera->position.y + camera->direction.y+5;
    ship->position.z = camera->position.z + camera->direction.z+5;*/
    Vector temp;
    temp.x = 1000;
    temp.y = 1000;
    temp.z = 1000;
    ship->position = scalarMultiply(camera->direction, camera->position, temp, dt);
    float dot = dotProduct(camera->direction,ship->front);
    ship->direction = camera->direction;
}

void updateBullet() {
    for (int i = 0; i < bullets.size(); i++) {
        Vector tempVector;
        tempVector.x = bullets.at(i).speed;
        tempVector.y = bullets.at(i).speed;
        tempVector.z = bullets.at(i).speed;
        //checkBullet(&bullets.at(i));
        bullets.at(i).position = scalarMultiply(bullets.at(i).direction, bullets.at(i).position, tempVector, dt);
        //checkBulletAsteroid(&astroidVector, &bullet->at(i));
        if (!bullets.at(i).alive) {
            bullets.erase(bullets.begin() + i);
        }
        print(bullets.at(i).position);
    }
}
void updatePosition(Camera* camera, float dt) {
    float xrotrad, yrotrad;
    Vector velocity;
    velocity.x = 100;
    velocity.y = 100;
    velocity.z = 100;
    if (input == 'w') {
        /*yrotrad = (camera.pitch / 180.0f * 3.141592654f);
        xrotrad = (camera.yaw / 180.0f * 3.141592654f);
        camera.position.x += (float)(sin(yrotrad));
        camera.position.y += (float)(sin(xrotrad));
        camera.position.z -= (float)(cos(yrotrad));*/

        camera->position = scalarMultiply(camera->direction, camera->position, velocity, dt);
        input = ' ';
    }
    if (input == 'd') {
        input = ' ';
    }
    if (input == 's') {
        velocity.x = -100;
        velocity.y = -100;
        velocity.z = -100;
        camera->position = scalarMultiply(camera->direction, camera->position, velocity, dt);
        input = ' ';
    }
    if (input == 'a') {
        input = ' ';
    }
}
void checkplayer() {
    if (camera.position.x > ARENA_SIZE) {
        die = true;
    }
    if (camera.position.y > ARENA_SIZE) {
        die = true;
    }
    if (camera.position.z > ARENA_SIZE) {
        die = true;
    }
    if (camera.position.x < -ARENA_SIZE) {
        die = true;
    }
    if(camera.position.y < -ARENA_SIZE) {
        die = true;
    }
    if (camera.position.z < -ARENA_SIZE) {
        die = true;
    }
}

void on_key_press(unsigned char key, int x, int y)
{
    switch (key)
    {
        // toggle axes display
    case 'X':
    case 'x':
        g_show_axes = !g_show_axes;
        break;
        // rotate platform clockwise
    case 'A':
    case 'a':
        g_platform_rotation -= PLATFORM_ROTATION_DELTA;
        input = 'a';
        break;
        // rotate platform counter-clockwise
    case 'D':
    case 'd':
        g_platform_rotation += PLATFORM_ROTATION_DELTA;
        input = 'd';
        break;
        // rotate cannon up
    case 'W':
    case 'w':
        g_cannon_rotation += CANNON_ROTATION_DELTA;
        input = 'w';
        break;
        // rotate cannon down
    case 'S':
    case 's':
        g_cannon_rotation -= CANNON_ROTATION_DELTA;
        input = 's';
        break;
        // rotate camera clockwise
    case '<':
    case ',':
        g_camera_rotation -= CAMERA_ROTATION_DELTA;
        break;
        // rotate camera counterclockwise
    case '>':
    case '.':
        g_camera_rotation += CAMERA_ROTATION_DELTA;
        break;
        // quit
    case KEY_ESC:
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}

void move_camera() {
    gluLookAt(camera.position.x, camera.position.y, camera.position.z,
        camera.position.x+camera.pitch, camera.position.y+camera.yaw, camera.position.z+camera.roll,
        0, 1, 0);
    //std::cout << camera.direction.x << "  y: " << camera.direction.y << std::endl;
}
void place_camera()
{
    gluLookAt(g_camera_position_x, g_camera_position_y, g_camera_position_z,
        0, 0, WALL_Z,
        0, 1, 0);

}

void render_frame()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    move_camera();
    //place_camera();
    //cameraTest();
    displaySkybox();
    //draw_wall();
    draw_axes();
    //draw_cannon();
    //draw_platform();
    draw_Arena(&g_arena);
    draw_ship();
    drawAsteroids(&asteroids);
    drawBullet();
    //rawRing();
    //CreateSphere(asteroids.at(0).position,1,ASTEROID_DIVISIONS);
}

void on_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    render_frame();
    glutSwapBuffers();
    int err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error: %s\n", gluErrorString(err));
    }
}

void on_idle()
{
    float cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    dt = cur_time - g_last_time;
    updateAsteroid(&asteroids);
    updateBullet();
    checkBullet();
    checkInside();
    checkAsteroid();
    checkplayer();
    if (die) {
        waves = 0;
        Vector tempPos;
        tempPos.x = 0;
        tempPos.y = 20;
        tempPos.z = 0;
        Vector tempDir;
        tempDir.x = 0;
        tempDir.y = 0;
        tempDir.z = 1;
        camera.position = tempPos;
        camera.direction = tempDir;
        camera.pitch = 0;
        camera.yaw = 0;
        camera.roll = 0;
        asteroids.clear();
        die = false;
    }
    if (asteroids.size()==0) {
        completedWave = true;
    }
    else {
        completedWave = false;
    }
    if (completedWave) {
        std::cout << waves << std::endl;
        waves++;
        generateAsteroids();

    }
    //update_camera_position();
    glutPostRedisplay();
    glutPassiveMotionFunc(moveMouse);
    //glutWarpPointer(2560 / 2, 1440 / 2);
    g_last_time = cur_time;
    updatePosition(&camera, dt);
    updateShip(&ship, &camera);
    ship.checkShip();
}

void on_reshape(int w, int h)
{
    // Setup the viewport transform
    glViewport(0, 0, w, h);

    // Setup the projection transform
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 1000.0);
}

void init_lighting()
{
    float ambient0[] = {1.0, 1.0, 1.0, 1.0};
    float diffuse0[] = {1.0, 1.0, 1.0, 1.0};
    float specular0[] = {1.0, 1.0, 1.0, 1.0};

    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    // glShadeModel(GL_FLAT);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glEnable(GL_LIGHT0);
}

void initTime() {
    g_last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void init_app(int* argcp, char** argv)
{
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1300, 1000);
    
    glutCreateWindow("s3781767 Assignment 2");
    //glutFullScreen();
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_key_press);
    glutMouseFunc(on_mousepress);
    glutDisplayFunc(on_display);
    glutIdleFunc(on_idle);
    init_lighting();
    loadSkybox();
    initAssets();
    randomPosition();
    initShip();
    //glutWarpPointer(1920 / 2, 1080 / 2);

    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    initTime();
    // to see wireframe mode ...
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv)
{
    init_app(&argc, argv);
    glutMainLoop();
    return (EXIT_SUCCESS);
}
