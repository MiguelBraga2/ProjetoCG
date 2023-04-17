#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "translation.hpp"


Translation::Translation(float x, float y, float z, float duration, bool align) : Transformation(x, y, z), duration(duration), align(align) {
}

void Translation::applyTransformation(){
    glTranslatef(this->getX(), this->getY(), this->getZ());
}

void Translation::applyTransformationToPoint(Point* base, float* radius) {
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}