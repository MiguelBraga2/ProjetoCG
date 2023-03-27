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
    Point d {};

public:
    Camera();
    Camera(const Point &position, const Point &lookAtPosition, const Point &upVector, float fov, float near, float far);
    void incrementAlfa();
    void decrementAlfa();
    void incrementBeta();
    void decrementBeta();
    void incrementRadius();
    void decrementRadius();
    Point getPosition();
    Point getLookAtPosition();
    Point getUpVector();
    float getFov();
    float getNear();
    float getFar();

    float getAlfa();

    void setAlfa(float alfa);

    float getBeta();

    void setBeta(float beta);

    float getCameraRadius();

    void setCameraRadius(float cameraRadius);

    void incrementIncrement();
    void decrementIncrement();
    void moveForwards();
    void moveBackwards();

    void setPosition(Point position);

    void moveLeft();
    void moveRight();
    void changeMode();

private:
    /**
     * Convert spherical coordenates (alpha and beta) into x, y, z coordinates
     */
    void spherical2Cartesian();

public:
    Point getD();

    int getMode();

private:
    void calculateBeta();
    void calculateAlpha();
    void calculateDirection();
};

#endif
