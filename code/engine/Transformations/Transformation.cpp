#include "Transformation.h"

     Transformation:: Transformation(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float Transformation::getX() const {
        return x;
    }

    void Transformation::setX(float x) {
        Transformation::x = x;
    }

    float Transformation::getY() const {
        return y;
    }

    void Transformation::setY(float y) {
        Transformation::y = y;
    }

    float Transformation::getZ() const {
        return z;
    }

    void Transformation::setZ(float z) {
        Transformation::z = z;
    }