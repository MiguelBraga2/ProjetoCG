#include<iostream>
#include "point.hpp"
#include <string>
#include "math.h"
using namespace std;

Point::Point()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}


Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Point::getX()
{
    return this->x; 
};

float Point::getY() 
{
    return this->y;
};

float Point::getZ()
{ 
    return this->z; 
};

void Point::setX(float x) {
    this->x = x;
}

void Point::setY(float y) {
    this->y = y;
}

void Point::setZ(float z) {
    this->z = z;
}

void Point::setPoint(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;

}

Point Point::crossProduct(Point v1, Point v2) {
    Point p;
    p.x = v1.getY()*v2.getZ() - v1.getZ()*v2.getY();
    p.y = v1.getZ()*v2.getX() - v1.getX()*v2.getZ();
    p.z = v1.getX()*v2.getY() - v1.getY()*v2.getX();
    return p;
}

float Point::getSize(){
    return abs(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
}

float Point::normalize(){
    float size = this->getSize();
    this->x = this->x/size;
    this->y = this->y/size;
    this->z = this->z/size;
}

string Point::toString()
{
    return to_string(this->x) + " " + to_string(this->y) + " " + to_string(this->z);
}
