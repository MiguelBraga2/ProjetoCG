#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

#include "../../shared/point.hpp"

class Plane {
public:
    Plane(float a, float b, float c, float d);
    bool inRightSide(Point p);

private:
    float a;
    float b;
    float c;
    float d;
};


#endif 
