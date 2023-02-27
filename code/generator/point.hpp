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
    string toString();
};

#endif