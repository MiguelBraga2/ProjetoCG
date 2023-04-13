#ifndef CAMERA
#define CAMERA

#include <iostream>
#include "../shared/point.hpp"
using namespace std;

class Camera
{
private:
    Point position;
    Point lookAtPosition;
    Point upVector;
    float fov, near, far;
    float increment;
    int mode; // 0 - Explorer mode, 1 - FPS mode
    float alfa, beta, cameraRadius;
    float step;
    Point d {};
    int tracking = 0;
    int startX, startY;

public:
    Camera();
    Camera(const Point &position, const Point &lookAtPosition, const Point &upVector, float fov, float near, float far);

    void setPosition(Point position);

    void incrementAlfa();
    void decrementAlfa();
    void incrementBeta();
    void decrementBeta();

    float getCameraRadius();

    void incrementRadius();
    void decrementRadius();
    void incrementIncrement();
    void decrementIncrement();
    void moveForwards();
    void moveBackwards();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void changeMode(int mode); // -1 goes to the next
    void placeCamera();
    void updateMouseAngles(int button, int state, int xx, int yy);
    void processMouseMotion(int xx, int yy);
    const unsigned char* toString();
    Point getPosition();
    Point getLookAtPosition();
    Point getUpVector();
    float getFov();
    float getNear();
    float getFar();

private:
    /**
     * Convert spherical coordenates (alpha and beta) into x, y, z coordinates
     */
    void spherical2Cartesian();

public:
    void setLookAtPosition(Point lookAtPosition);

    void setCameraRadius(float cameraRadius);

private:
    void calculateDirection();
    void calculateAlfa();
    void calculateBeta();
    void calculateSpherical();
};

#endif
