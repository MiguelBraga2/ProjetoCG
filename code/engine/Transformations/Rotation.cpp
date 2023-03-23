#include <GL/glut.h>
#include "Rotation.h"

Rotation::Rotation(float x, float y, float z, float angle) : Transformation(x, y, z), angle(angle) {}

void Rotation::applyTransformation() {
        glRotatef(this->getAngle(), this->getX(), this->getY(), this->getZ());
    }

    float Rotation::getAngle() {
        return angle;
    }

    void Rotation::setAngle(float angle) {
        Rotation::angle = angle;
    }