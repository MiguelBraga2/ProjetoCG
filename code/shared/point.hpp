#ifndef POINT
#define POINT

#include <iostream>
using namespace std;

class Point
{
private:
    float x, y, z;

public:
    Point();
    Point(float x, float y, float z);
    float getX();
    float getY();
    float getZ();
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setPoint(float x, float y, float z);
    static Point crossProduct(Point v1, Point v2);
    float getSize();
    void normalize();
    string toString();
    static void multMatrixMatrix(float* m1, int m1X, int m1Y, float* m2, int m2X, int m2Y, float** r);
};

#endif
