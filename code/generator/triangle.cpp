#include<iostream>
using namespace std;

#include "triangle.hpp"
#include "point.hpp"

Triangle::Triangle()
{
    this->p1 = Point();
    this->p2 = Point();
    this->p3 = Point();

}

Triangle::Triangle(Point p1, Point p2, Point p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}

Point Triangle::getP1() 
{ 
    return p1; 
};

Point Triangle::getP2() 
{ 
    return p2;
};

Point Triangle::getP3() 
{
    return p3; 
};


string Triangle::toString() {
    return "P1:" + this->p1.toString() + "\nP2: " + this->p2.toString() + "\nP3: " + this->p3.toString();
}
