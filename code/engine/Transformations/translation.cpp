#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "translation.hpp"


Translation::Translation(float x, float y, float z) : Transformation(x, y, z) {
}

void Translation::applyTransformation(){
    glTranslatef(this->getX(), this->getY(), this->getZ());
}

void Translation::calculateExternalPoint(Point* base){
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}
