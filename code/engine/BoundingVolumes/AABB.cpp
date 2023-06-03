#include "AABB.hpp"
#include "plane.hpp"
#include "../../shared/matrixOp.hpp"
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define INDEX(row, col) ((col) * 4 + (row) - 5)

AABB::AABB(Point* corners) {
    for(int i = 0; i < 8; i++) {
        this->corners[i].setPoint(corners[i].getX(), corners[i].getY(), corners[i].getZ());
    }
}

AABB::AABB() {
}

void AABB::setCorners(Point* corners) {
    for(int i = 0; i < 8; i++) {
        this->corners[i].setPoint(corners[i].getX(), corners[i].getY(), corners[i].getZ());
    }
}   

Point* AABB::getCorners() {
    return this->corners;
}

bool AABB::test(Plane *planes, float* actual_matrix) {
    bool ret = false;

    for(int i=0; i<6 && !ret; i++) {

        for(int j=0; j<8 && !ret; j++) {
            ret = planes[i].inRightSide(this->corners[j]);
        }

        if (!ret) return false;
        else ret = false;
    }

    return true;
}