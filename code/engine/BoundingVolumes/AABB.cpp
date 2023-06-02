#include "AABB.hpp"
#include "plane.hpp"
#include "../../shared/matrixOp.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define INDEX(col, row) ((col) * 4 + (row) - 5)

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


bool AABB::test(float *matrix) {
    // testar se para todos existe um ponto do lado certo
    bool ret = false;
    Point cs[8];
    for(int i = 0; i < 8; i++) {
        float v[4] = {this->corners[i].getX(), this->corners[i].getY(), this->corners[i].getZ(), 0};
        float res[4] = {0, 0, 0, 0};
        multiplyMatrixVector(matrix, v, res);
        cs[i].setPoint(res[0], res[1], res[2]);
    }
 
    /*  left plane 
        a = a11 + a41 
        b = a12 + a42 
        c = a13 + a43
        d = a14 + a44
    */   
    Point p(matrix[INDEX(1, 1)] + matrix[INDEX(4, 1)], matrix[INDEX(1, 2)] + matrix[INDEX(4, 2)], matrix[INDEX(1, 3)] + matrix[INDEX(4, 3)]);
    float d = matrix[INDEX(1, 4)] + matrix[INDEX(4, 4)];
    d /= p.getSize();
    p.normalize();
    Plane lp(p.getX(), p.getY(), p.getZ(), d);

    for(int i = 0; i < 8 && !ret; i++) {
        ret = lp.inRightSide(this->corners[i]);
    }

    if (!ret) {
        return false;
    } else {
        ret = false;
    }

    /* right plane
        a = -a11 + a41 
        b = -a12 + a42
        c = -a13 + a43
        d = -a14 + a44
    */
    Point p2(-matrix[INDEX(1, 1)] + matrix[INDEX(4, 1)], -matrix[INDEX(1, 2)] + matrix[INDEX(4, 2)], -matrix[INDEX(1, 3)] + matrix[INDEX(4, 3)]);
    float d2 = -matrix[INDEX(1, 4)] + matrix[INDEX(4, 4)];
    d2 /= p2.getSize();
    p2.normalize();
    Plane rp(p2.getX(), p2.getY(), p2.getZ(), d2);
    
    for(int i = 0; i < 8 && !ret; i++) {
        ret = rp.inRightSide(this->corners[i]);
    }

    if (!ret) {
        return false;
    } else {
        ret = false;
    }

    /* top plane  
        a = -a21 + a41 
        b = -a22 + a42
        c = -a23 + a43
        d = -a24 + a44
    */
    Point p3(-matrix[INDEX(2, 1)] + matrix[INDEX(4, 1)], -matrix[INDEX(2, 2)] + matrix[INDEX(4, 2)], -matrix[INDEX(2, 3)] + matrix[INDEX(4, 3)]);
    float d3 = -matrix[INDEX(2, 4)] + matrix[INDEX(4, 4)];
    d3 /= p3.getSize();
    p3.normalize();
    Plane tp(p3.getX(), p3.getY(), p3.getZ(), d3);

    for(int i = 0; i < 8 && !ret; i++) {
        ret = tp.inRightSide(this->corners[i]);
    }

    if (!ret) {
        return false;
    } else {
        ret = false;
    }

    /* bottom plane
        a = a21 + a41 
        b = a22 + a42
        c = a23 + a43
        d = a24 + a44
    */
    Point p4(matrix[INDEX(2, 1)] + matrix[INDEX(4, 1)], matrix[INDEX(2, 2)] + matrix[INDEX(4, 2)], matrix[INDEX(2, 3)] + matrix[INDEX(4, 3)]);
    float d4 = matrix[INDEX(2, 4)] + matrix[INDEX(4, 4)];
    d4 /= p4.getSize();
    p4.normalize();
    Plane bp(p4.getX(), p4.getY(), p4.getZ(), d4);

    for(int i = 0; i < 8 && !ret; i++) {
        ret = bp.inRightSide(this->corners[i]);
    }
    
    if (!ret) {
        return false;
    } else {
        ret = false;
    }

    /* far plane
        a = -a31 + a41 
        b = -a32 + a42
        c = -a33 + a43
        d = -a34 + a4
    */
    Point p5(-matrix[INDEX(3, 1)] + matrix[INDEX(4, 1)], -matrix[INDEX(3, 2)] + matrix[INDEX(4, 2)], -matrix[INDEX(3, 3)] + matrix[INDEX(4, 3)]);
    float d5 = -matrix[INDEX(3, 4)] + matrix[INDEX(4, 4)];
    d5 /= p5.getSize();
    p5.normalize();
    Plane fp(p5.getX(), p5.getY(), p5.getZ(), d5);

    for(int i = 0; i < 8 && !ret; i++) {
        ret = fp.inRightSide(this->corners[i]);
    }
    cout << ret << endl;

    if (!ret) {
        return false;
    } else {
        ret = false;
    }


    /* near plane 
        a = a31 + a41 
          b = a32 + a42
        c = a33 + a43
        d = a34 + a44 
    */
    Point p6(matrix[INDEX(3, 1)] + matrix[INDEX(4, 1)], matrix[INDEX(3, 2)] + matrix[INDEX(4, 2)], matrix[INDEX(3, 3)] + matrix[INDEX(4, 3)]);
    float d6 = matrix[INDEX(3, 4)] + matrix[INDEX(4, 4)];
    d6 /= p6.getSize();
    p6.normalize();
    Plane np(p6.getX(), p6.getY(), p6.getZ(), d6);

    for(int i = 0; i < 8 && !ret; i++) {
        ret = np.inRightSide(this->corners[i]);
    }
    
    return ret;
}
