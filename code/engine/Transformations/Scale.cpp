#include <GL/glut.h>
#include "Scale.h"

Scale::Scale(float x, float y, float z) {
    this->setX(x);
    this->setY(y);
    this->setZ(z);
}

void Scale::applyTransformation(){
        glScalef(this->getX(), this->getY(), this->getZ());
    }