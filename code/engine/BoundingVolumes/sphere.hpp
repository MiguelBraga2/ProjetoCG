#ifndef ENGINE_SPHERE_H
#define ENGINE_SPHERE_H

#include "volume.hpp"
#include "../../shared/point.hpp"

class Sphere : public Volume {
public:
    Sphere(float radius, Point center);
    bool test(float *matrix);
    Sphere();
    void setRadius(float radius);
    void setCenter(Point center);

private:
    float radius;
    Point center;
};


#endif 
