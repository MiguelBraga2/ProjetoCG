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

        float t = (translationTime - segmentTime * segment)/segmentTime;

        Point p1, p2, p3, p4;
        // 4 pontos de controle
        p1 = this->controlPoints[(segment-1)%this->controlPoints.size()];
        p2 = this->controlPoints[(segment)%this->controlPoints.size()];
        p3 = this->controlPoints[(segment+1)%this->controlPoints.size()];
        p4 = this->controlPoints[(segment+2)%this->controlPoints.size()];

        glBegin(GL_LINE_LOOP);
        glVertex3f(p1.getX(), p1.getY(), p1.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        glVertex3f(p4.getX(), p4.getY(), p4.getZ());
        glEnd();

        float M[4][4];
        float t_vector[4] = {t*t*t, t*t, t, 1};
        float t_vector_deriv[4] = {3*t*t, 2*t, 1, 0};
        float Aux[4][4] = {{-0.5, 1.5, -1.5, 0.5},
                           {1, -2.5, 2, -0.5},
                           {-0.5, 0, 0.5, 0},
                           {0, 1, 0, 0}};
        float P[4][4] = {{p1.getX(), p1.getY(), p1.getZ(), 1},
                         {p2.getX(), p2.getY(), p2.getZ(), 1},
                         {p3.getX(), p3.getY(), p3.getZ(), 1},
                         {p4.getX(), p4.getY(), p4.getZ(), 1}};
        float* pos;
        float* aux;
        Point::multMatrixMatrix(t_vector, 1, 4, *Aux, 4, 4, &aux);
        Point::multMatrixMatrix(aux, 1, 4, *P, 4, 4, &pos);

        glTranslatef(pos[0], pos[1], pos[2]);

        float* deriv;
        Point::multMatrixMatrix(t_vector_deriv, 1, 4, *Aux, 4, 4, &aux);
        Point::multMatrixMatrix(aux, 1, 4, *P, 4, 4, &deriv);

        Point axX, axZ;
        axX = Point(deriv[0], deriv[1], deriv[2]);
        axX.normalize();

        axZ = Point::crossProduct(axX, this->yi);
        axZ.normalize();

        this->yi = Point::crossProduct(axZ, axX);
        this->yi.normalize();

        M[0][0] = axX.getX(); M[0][1] = axX.getY(); M[0][2] = axX.getZ(); M[0][3] = 0;
        M[1][0] = this->yi.getX(); M[1][1] = this->yi.getY(); M[1][2] = this->yi.getZ(); M[1][3] = 0;
        M[2][0] = axZ.getX(); M[2][1] = axZ.getY(); M[2][2] = axZ.getZ(); M[2][3] = 0;
        M[3][0] = 0; M[3][1] = 0; M[3][2] = 0; M[3][3] = 1;

        for(int i=0; i<4; i++){
            float o = aux[i];
            //cout << aux[i] << endl;
        }

        // Matrix with the columns being the position of the axis
        glMultMatrixf(*M);
    }

}

void Translation::applyTransformationToPoint(Point* base, float* radius) {
    base->setX(base->getX() + this->getX());
    base->setY(base->getY() + this->getY());
    base->setZ(base->getZ() + this->getZ());
}