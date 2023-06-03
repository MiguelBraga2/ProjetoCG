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

Volume* AABB::clone() {
    return new AABB(this->corners);
}


bool AABB::test(float *matrix, Camera *camera) {
    bool ret = false;
    Point cs[8];
    for (int i = 0; i < 8; i++) {
        float v[4] = { this->corners[i].getX(), this->corners[i].getY(), this->corners[i].getZ(), 1 };
        float res[4] = { 0, 0, 0, 0 };
        multiplyMatrixVector(matrix, v, res);
        cs[i].setPoint(res[0], res[1], res[2]);
    }

    // Left plane
    float a = matrix[INDEX(4, 1)] + matrix[INDEX(1, 1)];
    float b = matrix[INDEX(4, 2)] + matrix[INDEX(1, 2)];
    float c = matrix[INDEX(4, 3)] + matrix[INDEX(1, 3)];
    float d = matrix[INDEX(4, 4)] + matrix[INDEX(1, 4)];
    float l = sqrt(a * a + b * b + c * c);
    Plane lp(a/l, b/l, c/l, d/l);

    for (int i = 0; i < 8 && !ret; i++) {
        ret = lp.inRightSide(cs[i]);
    }

    if (!ret) return false;
    else ret = false;

    // Right plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(1, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(1, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(1, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(1, 4)];
    l = sqrt(a * a + b * b + c * c);
    Plane rp(a/l, b/l, c/l, d/l);

    for (int i = 0; i < 8 && !ret; i++) {
        ret = rp.inRightSide(cs[i]);
    }

    if (!ret) return false;
    else ret = false;

    // Top plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(2, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(2, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(2, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(2, 4)];
    l = sqrt(a * a + b * b + c * c);
    Plane tp(a/l, b/l, c/l, d/l);

    for (int i = 0; i < 8 && !ret; i++) {
        ret = tp.inRightSide(cs[i]);
    }

    if (!ret) return false;
    else ret = false;

    // Bottom plane
    a = matrix[INDEX(4, 1)] + matrix[INDEX(2, 1)];
    b = matrix[INDEX(4, 2)] + matrix[INDEX(2, 2)];
    c = matrix[INDEX(4, 3)] + matrix[INDEX(2, 3)];
    d = matrix[INDEX(4, 4)] + matrix[INDEX(2, 4)];
    l = sqrt(a * a + b * b + c * c);
    Plane bp(a/l, b/l, c/l, d/l);

    for (int i = 0; i < 8 && !ret; i++) {
        ret = bp.inRightSide(cs[i]);
    }

    if (!ret) return false;
    else ret = false;

    // Far plane
    a = matrix[INDEX(4, 1)] - matrix[INDEX(3, 1)];
    b = matrix[INDEX(4, 2)] - matrix[INDEX(3, 2)];
    c = matrix[INDEX(4, 3)] - matrix[INDEX(3, 3)];
    d = matrix[INDEX(4, 4)] - matrix[INDEX(3, 4)];
    l = sqrt(a * a + b * b + c * c);
    Plane fp(a/l, b/l, c/l, d/l);

        for (int i = 0; i < 8 && !ret; i++) {
            ret = fp.inRightSide(cs[i]);
        }

    if (!ret) return false;
    else ret = false;

    // Near plane
    a = matrix[INDEX(4, 1)] + matrix[INDEX(3, 1)];
    b = matrix[INDEX(4, 2)] + matrix[INDEX(3, 2)];
    c = matrix[INDEX(4, 3)] + matrix[INDEX(3, 3)];
    d = matrix[INDEX(4, 4)] + matrix[INDEX(3, 4)];
    l = sqrt(a * a + b * b + c * c);
    Plane np(a/l, b/l, c/l, d/l);

    for (int i = 0; i < 8 && !ret; i++) {
        ret = np.inRightSide(cs[i]);
    }

    return ret;
}
