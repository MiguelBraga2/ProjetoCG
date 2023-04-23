#include "matrixOp.hpp"
#include "point.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}


float length(float *v) {

    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void multiplyMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

void multiplyMatrixPointMatrix(float *m1, Point *m2, Point *res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res[i * 4 + j].sum(m2[k * 4 + j].scale(m1[i * 4 + k]));
            }
        }
    }
}

void multiplyPointMatrixMatrix(Point *m1, float *m2, Point *res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res[i * 4 + j].sum(m1[i * 4 + k].scale(m2[k * 4 + j]));
            }
        }
    }
}

void multiplyVectorPointMatrix(float *v, Point *m, Point *res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i].sum(m[j * 4 + i].scale(v[j]));
        }
    }
}

void multiplyPointVectorVector(Point *v1, float *v2, Point *res) {
    for (int i = 0; i < 4; i++) {
        res->sum(v1[i].scale(v2[i]));
    }
}
