#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "rotation.hpp"

Rotation::Rotation(float x, float y, float z, float angle) : Transformation(x, y, z), angle(angle) {
}

void Rotation::applyTransformation() {
    glRotatef(this->getAngle(), this->getX(), this->getY(), this->getZ());
}

float Rotation::getAngle() {
    return angle;
}

void Rotation::setAngle(float angle) {
    this->angle = angle;
}