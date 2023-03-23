#include <GL/glut.h>
#include "Translation.h"


    Translation::Translation(float x, float y, float z) {
        this->setX(x);
        this->setX(y);
        this->setX(z);
    }

    void Translation::applyTransformation(){
        glTranslatef(this->getX(), this->getY(), this->getZ());
    }
