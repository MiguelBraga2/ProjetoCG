#include "sphere.hpp"
#include "plane.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Sphere::Sphere(float radius, Point center) : radius(radius), center(center) {

}

Sphere::Sphere() {
}

void Sphere::setRadius(float radius) {
    this->radius = radius;
}

void Sphere::setCenter(Point center) {
    this->center.setPoint(center.getX(), center.getY(), center.getZ());
}

bool Sphere::test(float *matrix) {
    bool ret = true;
    
        
    return ret;
}
