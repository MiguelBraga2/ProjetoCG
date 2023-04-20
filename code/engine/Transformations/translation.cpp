#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

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

void Translation::getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float *pos, float *deriv) {

    // catmull-rom matrix
    float m[16] = {	-0.5f,  1.5f, -1.5f,  0.5f,
                          1.0f, -2.5f,  2.0f, -0.5f,
                         -0.5f,  0.0f,  0.5f,  0.0f,
                         0.0f,  1.0f,  0.0f,  0.0f};

    float p_0[3] = { p0.getX(), p0.getY(), p0.getZ()};
    float p_1[3] = { p1.getX(), p1.getY(), p1.getZ()};
    float p_2[3] = { p2.getX(), p2.getY(), p2.getZ()};
    float p_3[3] = { p3.getX(), p3.getY(), p3.getZ()};

    for (int i = 0; i < 3; i++) {
        // Compute A = M * P
        float P[4] = {p_0[i], p_1[i], p_2[i], p_3[i]};
        float *A;
        Point::multMatrixMatrix(m, 4, 4, P, 4, 1, &A);

        // Compute pos = T * A
        float T[4] { t*t*t , t*t, t, 1};
        pos[i] = T[0] * A[0] + T[1] * A[1] + T[2] * A[2] + T[3] * A[3];

        // compute deriv = T' * A
        float TL[4] = { 3 * t * t, 2 * t, 1, 0};
        deriv[i] = TL[0] * A[0] + TL[1] * A[1] + TL[2] * A[2] + TL[3] * A[3];
    }
}


// given  global t, returns the point in the curve
void Translation::getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {

    float point_count = this->controlPoints.size();
    float t = gt * point_count; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + this->controlPoints.size()-1)%this->controlPoints.size();
    indices[1] = (indices[0]+1)%this->controlPoints.size();
    indices[2] = (indices[1]+1)%this->controlPoints.size();
    indices[3] = (indices[2]+1)%this->controlPoints.size();

    getCatmullRomPoint(t, this->controlPoints[indices[0]], this->controlPoints[indices[1]], this->controlPoints[indices[2]], this->controlPoints[indices[3]], pos, deriv);
}

void Translation::renderCatmullRomCurve() {

    float pos[3];
    float deriv[3];

    glBegin(GL_LINE_LOOP);
    for (float gt = 0; gt < 1; gt += 0.01) {
        getGlobalCatmullRomPoint(gt, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();

}

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
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

        if (translationTime > duration) {
            int iterationsPassed = (int) (translationTime / duration);
            translationTime = translationTime - iterationsPassed * this->duration;
            startCounter = glutGet(GLUT_ELAPSED_TIME); // Set the counter to the current moment
        }

        float t = translationTime / duration;

        renderCatmullRomCurve();

        float pos[3];
        float deriv[3];
        getGlobalCatmullRomPoint(t, pos, deriv);
        glTranslatef(pos[0], pos[1], pos[2]);

        Point X(deriv[0], deriv[1], deriv[2]);
        X.normalize();

        Point Z = Point::crossProduct(X, yi);
        Z.normalize();

        Point yi = Point::crossProduct(Z, X);

        float m[16];
        float xi[3] = {X.getX(), X.getY(), X.getZ()};
        float yf[3] = {yi.getX(), yi.getY(), yi.getZ()};
        float zi[3] = {Z.getX(), Z.getY(), Z.getZ()};

        buildRotMatrix(xi, yf, zi, m);
        glMultMatrixf(m);
    }

}

void Translation::applyTransformationToPoint(Point* base, float* radius) {
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}