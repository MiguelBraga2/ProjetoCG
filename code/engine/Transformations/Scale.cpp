#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "scale.hpp"

Scale::Scale(float x, float y, float z) : Transformation(x,y,z) {

}

void Scale::applyTransformation(){
    glScalef(this->getX(), this->getY(), this->getZ());
}