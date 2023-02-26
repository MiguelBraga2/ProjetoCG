#ifndef TRIANGLE
#define TRIANGLE

#include "point.hpp"

typedef struct Triangle {
    Point x;
    Point y;
    Point z;
} Triangle;

void printTriangle(Triangle t);
#endif