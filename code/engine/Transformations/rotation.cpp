#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "rotation.hpp"
#include <math.h>

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

void Rotation::applyTransformationToPoint(Point* base, float* radius) {
    if (this->getX() == 0 && this->getY() == 0 && this->getZ() == 1){
        float baseX = base->getX(), baseY = base->getY();
        base->setX(baseX * cos(M_PI/180*angle) + baseY*(-sin(M_PI/180*angle)));
        base->setY(baseX * sin(M_PI/180*angle) + baseY * cos(M_PI/180*angle));
    }
    else if (this->getX() == 0 && this->getY() == 1 && this->getZ() == 0){
        float baseX = base->getX(), baseZ = base->getZ();
        base->setX(baseX * cos(M_PI/180*angle) + baseZ * sin(M_PI/180*angle));
        base->setZ(baseX * (-sin(M_PI/180*angle)) + baseZ * cos(M_PI/180*angle));
    }
    else if (this->getX() == 1 && this->getY() == 0 && this->getZ() == 0){
        float baseY = base->getY(), baseZ = base->getZ();
        base->setY(baseY * cos(M_PI/180*angle) + baseZ * (-sin(M_PI/180*angle)));
        base->setZ(baseY * sin(M_PI/180*angle) + baseZ * cos(M_PI/180*angle));
    }
}
