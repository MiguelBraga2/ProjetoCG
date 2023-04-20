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
    yi = Point(0, 1, 0); // Initial inclination
    startCounter = 0;
}

void Translation::applyTransformation(){
    if (this->duration == 0){
        glTranslatef(this->getX(), this->getY(), this->getZ());
    }
    else {
        if (startCounter == 0) {
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Time since the beginning of the program
        }

        double currentTime = glutGet(GLUT_ELAPSED_TIME); // Current time
        double translationTime = (currentTime - startCounter) / 1000; // In seconds
        double segmentTime = this->duration / this->controlPoints.size(); // Time for each segment
        int segment = (int) (translationTime / segmentTime);

        if (translationTime > duration) {
            int iterationsPassed = (int) (translationTime / duration);
            translationTime = translationTime - iterationsPassed * this->duration;
            segment = (int) (translationTime / segmentTime);
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Set the counter to the current moment
        }
        int numIterations = this->controlPoints.size()-3;

        for(int i=0; i<numIterations; i++){
            // 4 pontos de controle
            Point p1 = this->controlPoints[(i+segment-1)%this->controlPoints.size()];
            Point p2 = this->controlPoints[(i+segment)%this->controlPoints.size()];
            Point p3 = this->controlPoints[(i+segment+1)%this->controlPoints.size()];
            Point p4 = this->controlPoints[(i+segment+2)%this->controlPoints.size()];
        }

        //float M[4][4];

        // Matrix with the columns being the position of the axis
        //glMultMatrixf(*M);
    }

}

void Translation::applyTransformationToPoint(Point* base, float* radius) {
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}