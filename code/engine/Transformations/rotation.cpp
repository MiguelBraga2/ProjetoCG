#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "rotation.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

Rotation::Rotation(float x, float y, float z, float angle, double startCounter, float duration360) : Transformation(x, y, z), angle(angle), startCounter(startCounter), duration360(duration360) {
}

void Rotation::applyTransformation() {
    double rAngle;
    if (duration360 != 0) {
        if (startCounter == 0) {
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Time since the beginning of the program
        }

        double currentTime = glutGet(GLUT_ELAPSED_TIME); // Current time
        double rotationTime = (currentTime - startCounter) / 1000; // In seconds

        if (rotationTime > duration360) {
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Set the counter to the current moment
        }

        rAngle = (double)rotationTime * 360 / duration360; // Angle in radians
    }
    else rAngle = this->getAngle();
    
    glRotatef(rAngle, this->getX(), this->getY(), this->getZ());
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
    std::cout << base->toString() << endl;
}
