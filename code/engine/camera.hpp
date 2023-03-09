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
    void incrementIncrement();
    void decrementIncrement();

private:
    /**
     * Convert spherical coordenates (alpha and beta) into x, y, z coordinates
     */
    void spherical2Cartesian();
    void calculateBeta();
    void calculateAlpha();
};

#endif
