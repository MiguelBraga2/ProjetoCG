#include "Rotation.h"

Rotation::Rotation(float x, float y, float z, float angle) : Transformation(x, y, z) {
    this->angle = angle;
}

float Rotation::getAngle() const {
    return angle;
}

void Rotation::setAngle(float angle) {
    Rotation::angle = angle;
}

string Rotation::toString(){
    return "Rotation";
}
