#ifndef ENGINE_SPHERE_H
#define ENGINE_SPHERE_H

#include "volume.hpp"
#include "../../shared/point.hpp"
#include "../camera.hpp"

class Sphere : public Volume {
public:
    Sphere(float radius, Point center);
    bool test(float *matrix, Camera *camera);
    Sphere();
    void setRadius(float radius);
    float getRadius();
    void setCenter(Point center);
    Point getCenter();
    Volume *clone();

private:
    float radius;
    Point center;
};


#endif 
