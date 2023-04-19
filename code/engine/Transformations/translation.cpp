#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "translation.hpp"


Translation::Translation(float x, float y, float z, float duration, bool align, vector<Point> controlPoints) : Transformation(x, y, z), duration(duration), align(align) {
    for(Point p: controlPoints){
        this->controlPoints.push_back(p);
    }
}

void Translation::applyTransformation(){
    if (this->duration == 0){
        glTranslatef(this->getX(), this->getY(), this->getZ());
    }
    else {
        int numIterations = this->controlPoints.size()-3;
        float iterationInterval = this->duration / numIterations;


        for(int i=0; i<numIterations; i++){
            // 4 pontos de controle
            Point p1 = this->controlPoints[i];
            Point p2 = this->controlPoints[i+1];
            Point p3 = this->controlPoints[i+2];
            Point p4 = this->controlPoints[i+3];
        }
    }

}

void Translation::applyTransformationToPoint(Point* base, float* radius) {
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}