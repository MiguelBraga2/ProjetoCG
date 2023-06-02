#include "plane.hpp"


Plane::Plane(float a, float b, float c, float d) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}
    
bool Plane::inRightSide(Point p) {
    return (a * p.getX() + b * p.getY() + c * p.getZ() + d) > 0;
}
