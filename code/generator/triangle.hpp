#ifndef TRIANGLE
#define TRIANGLE

#include <iostream>
#include "point.hpp"
using namespace std;


class Triangle
{
private:
    Point p1;
    Point p2;
    Point p3;

public:
    Triangle();
    Triangle(Point p1, Point p2, Point p3);
    Point getP1();
    Point getP2();
    Point getP3();
    string toString();
};

#endif